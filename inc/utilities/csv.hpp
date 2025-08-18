/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   csv.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:36:01 by lagea             #+#    #+#             */
/*   Updated: 2025/08/18 18:18:15 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CSV_HPP
# define CSV_HPP

#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <cctype> 

class Csv
{
	public:
		Csv();
		Csv(const Csv &) = delete;
		Csv(Csv &&) noexcept;
		Csv& operator=(const Csv &) = delete;
		Csv& operator=(Csv &&) noexcept;
		~Csv() noexcept;

		void load(const std::string &);
		void save(const std::string &) const;

		void addRow(const std::vector<std::string> &);
		void setHeader(const std::vector<std::string> &);
		
		const std::vector<std::vector<std::string>>& getData() const;
		const std::vector<std::string>& getHeader() const;

		size_t rowCount() const;
		size_t columnCount() const;

		void clear();

		std::vector<std::string> &operator[](size_t);
		const std::vector<std::string> &operator[](size_t) const;

	private:
		std::vector<std::vector<std::string>> _data;
		std::vector<std::string> _header;
		
		void parseLine(const std::string &, std::vector<std::string> &);
		std::string formatLine(const std::vector<std::string> &) const;
		void trim(std::string &) const;
		bool isQuoted(const std::string &) const;
};

#endif