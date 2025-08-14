/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   csv.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:36:01 by lagea             #+#    #+#             */
/*   Updated: 2025/08/14 18:11:15 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CSV_HPP
# define CSV_HPP

#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <cctype> // for std::isspace

class CSV
{
	public:
		CSV();
		CSV(const CSV &) = delete;
		CSV(CSV &&) noexcept;
		CSV& operator=(const CSV &) = delete;
		CSV& operator=(CSV &&) noexcept;
		~CSV() noexcept;

		void load(const std::string &filename);
		void save(const std::string &filename) const;

		void addRow(const std::vector<std::string> &row);
		void setHeader(const std::vector<std::string> &header);
		
		const std::vector<std::vector<std::string>>& getData() const;
		const std::vector<std::string>& getHeader() const;

		size_t rowCount() const;
		size_t columnCount() const;

		void clear();

		std::vector<std::string> &operator[](size_t index);
		const std::vector<std::string> &operator[](size_t index) const;

	private:
		std::vector<std::vector<std::string>> _data;
		std::vector<std::string> _header;
		
		void parseLine(const std::string &line, std::vector<std::string> &out);
		std::string formatLine(const std::vector<std::string> &line) const;
		void trim(std::string &str) const;
		bool isQuoted(const std::string &str) const;
};

#endif