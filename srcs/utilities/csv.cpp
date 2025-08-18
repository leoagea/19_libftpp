/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   csv.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:36:24 by lagea             #+#    #+#             */
/*   Updated: 2025/08/18 18:20:28 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/utilities/csv.hpp"

/* Public Methods */

Csv::Csv() : _data(), _header()
{
}

Csv::Csv(Csv &&other) noexcept : _data(std::move(other._data)), _header(std::move(other._header))
{
}

Csv& Csv::operator=(Csv &&other) noexcept
{
	if (this != &other) {
		_data = std::move(other._data);
		_header = std::move(other._header);
	}
	return *this;
}

Csv::~Csv() noexcept
{
}

void Csv::load(const std::string &filename)
{
	std::ifstream file(filename);
	if (!file.is_open())
		throw std::runtime_error("Could not open file: " + filename);
	
	std::string line;
	_data.clear();
	_header.clear();

	if (std::getline(file, line))
		parseLine(line, _header);

	while (std::getline(file, line)) {
		std::vector<std::string> row;
		parseLine(line, row);
		_data.push_back(std::move(row));
	}

	file.close();
}

void Csv::save(const std::string &filename) const
{
	std::ofstream file(filename);
	if (!file.is_open())
		throw std::runtime_error("Could not open file for writing: " + filename);

	if (!_header.empty())
		file << formatLine(_header) << "\n";

	for (const auto &row : _data)
		file << formatLine(row) << "\n";

	file.close();
}

void Csv::addRow(const std::vector<std::string> &row)
{
	if (row.empty())
		throw std::invalid_argument("Cannot add an empty row");

	_data.push_back(row);
}

void Csv::setHeader(const std::vector<std::string> &header)
{
	if (header.empty())
		throw std::invalid_argument("Header cannot be empty");

	_header = header;
}

const std::vector<std::vector<std::string>>& Csv::getData() const
{
	return _data;
}

const std::vector<std::string>& Csv::getHeader() const
{
	return _header;
}

size_t Csv::rowCount() const
{
	return _data.size();
}

size_t Csv::columnCount() const
{
	return _header.size();
}

void Csv::clear()
{
	_data.clear();
	_header.clear();
}

std::vector<std::string> &Csv::operator[](size_t index)
{
	if (index >= _data.size())
		throw std::out_of_range("Index out of range");

	return _data[index];
}

const std::vector<std::string> &Csv::operator[](size_t index) const
{
	if (index >= _data.size())
		throw std::out_of_range("Index out of range");

	return _data[index];
}

/* Private Methods */

void Csv::parseLine(const std::string &line, std::vector<std::string> &out)
{
	out.clear(); 
	std::string cell;
	bool inQuotes = false;
	bool wasQuoted = false;
	
	for (size_t i = 0; i < line.size(); ++i) {
		char c = line[i];
		
		if (c == '"') {
			if (inQuotes && i + 1 < line.size() && line[i + 1] == '"') {
				cell += '"'; 
				++i; 
			} else {
				inQuotes = !inQuotes; 
				if (!inQuotes) 
					wasQuoted = true; 
			}
		} 
		else if (c == '\\' && i + 1 < line.size() && line[i + 1] == 'n') {
			cell += '\n';
			++i;
		}
		else if (c == ',' && !inQuotes) {
			if (!wasQuoted)
				trim(cell);
			out.push_back(std::move(cell));
			cell.clear();
		} 
		else
			cell += c;
	}
	
	if (!wasQuoted)
		trim(cell);

	out.push_back(std::move(cell));
}

std::string Csv::formatLine(const std::vector<std::string> &line) const
{
	std::string formatted;
	for (size_t i = 0; i < line.size(); ++i) {
		const auto &cell = line[i];
		
		if (isQuoted(cell)) {
			formatted += "\"";
			for (char c : cell) {
				if (c == '"')
					formatted += "\"\"";
				else if (c == '\n')
					formatted += "\\n";
				else 
					formatted += c;
			}
			formatted += "\"";
		} 
		else
			formatted += cell;
		
		if (i < line.size() - 1)
			formatted += ",";
	}
	return formatted;
}

void Csv::trim(std::string &str) const
{
	str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char ch) { return !std::isspace(ch); }));
	str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) { return !std::isspace(ch);}).base(), str.end());
}

bool Csv::isQuoted(const std::string &str) const
{
	return str.find(',') != std::string::npos || 
		   str.find('\n') != std::string::npos || 
		   str.find('"') != std::string::npos ||
		   (!str.empty() && (str.front() == ' ' || str.back() == ' '));
}
