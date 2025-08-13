/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uuid.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:26:56 by lagea             #+#    #+#             */
/*   Updated: 2025/08/13 18:56:59 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/datastructures/uuid.hpp"

UUID::UUID()
{
	_data.fill(0);
}

UUID::UUID(UUID &&other) noexcept : _data(std::move(other._data))
{
}

UUID::UUID(const std::string &str)
{
	if (!parseFromString(str))
		throw std::invalid_argument("Invalid UUID string format");

}

UUID& UUID::operator=(UUID &&other) noexcept
{
	if (this != &other) {
		_data = std::move(other._data);
	}
	return *this;
}

UUID::~UUID() noexcept
{
}

UUID UUID::generate()
{
	UUID uuid;
	uuid._generateV4();
	return uuid;
}

UUID UUID::generateV1()
{
	UUID uuid;
	uuid._generateV1();
	return uuid;
}

UUID UUID::generateV4()
{
	UUID uuid;
	uuid._generateV4();
	return uuid;
}

UUID UUID::null()
{
	return UUID();
}

std::string UUID::toString() const
{
	std::ostringstream oss;
	for (size_t i = 0; i < 16; ++i) {
		oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(_data[i]);
		if (i == 3 || i == 5 || i == 7 || i == 9) 
			oss << '-';
	}
	return oss.str();
}

std::string UUID::toString(bool p_uppercase) const
{
	std::string str = toString();
	if (p_uppercase) 
		std::transform(str.begin(), str.end(), str.begin(), ::toupper);
	else 
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);

	return str;
}

bool UUID::isNull() const noexcept
{
	return std::all_of(_data.begin(), _data.end(), [](uint8_t byte) { return byte == 0; });
}

bool UUID::isValid() const noexcept
{
	// A valid UUID must have a version and variant set
	int ver = version();
	return (ver >= 1 && ver <= 5) && ((_data[8] & 0xC0) == 0x80);
}

int UUID::version() const noexcept
{
	// Version is stored in the 7th byte (index 6)
	return (_data[6] >> 4) & 0x0F;
}

int UUID::variant() const noexcept
{
	// Variant is stored in the 9th byte (index 8)
	if ((_data[8] & 0xC0) == 0x80) 
		return 2; // RFC 4122 variant

	return -1; // Unknown variant
}

bool UUID::operator==(const UUID &other) const noexcept
{
	return _data == other._data;
}

bool UUID::operator!=(const UUID &other) const noexcept
{
	return !(*this == other);
}	

bool UUID::operator<(const UUID &other) const noexcept
{
	return _data < other._data;
}

bool UUID::operator<=(const UUID &other) const noexcept
{
	return _data <= other._data;
}

bool UUID::operator>(const UUID &other) const noexcept
{
	return _data > other._data;
}

bool UUID::operator>=(const UUID &other) const noexcept
{
	return _data >= other._data;
}

size_t UUID::hash() const noexcept
{
	size_t hash = 0;
	for (const auto &byte : _data)
		hash = (hash * 31) ^ static_cast<size_t>(byte);

	return hash;
}

const std::array<uint8_t, 16> &UUID::data() const noexcept
{
	return _data;
}

void UUID::setData(const std::array<uint8_t, 16> &data) noexcept
{
	_data = data;
}

std::ostream& operator<<(std::ostream &os, const UUID &uuid)
{
	os << uuid.toString();
	return os;
}

void UUID::_generateV1()
{
	// Generate a version 1 UUID (timestamp-based)

	auto now = std::chrono::steady_clock::now();
	auto duration = now.time_since_epoch();
	auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();

	_data[0] = static_cast<uint8_t>(nanoseconds & 0xFF);
	_data[1] = static_cast<uint8_t>((nanoseconds >> 8) & 0xFF);
	_data[2] = static_cast<uint8_t>((nanoseconds >> 16) & 0xFF);
	_data[3] = static_cast<uint8_t>((nanoseconds >> 24) & 0xFF);
	_data[4] = static_cast<uint8_t>((nanoseconds >> 32) & 0xFF);
	_data[5] = static_cast<uint8_t>((nanoseconds >> 40) & 0xFF);
	_data[6] = static_cast<uint8_t>(((nanoseconds >> 48) & 0x0F) | 0x10);
	_data[7] = static_cast<uint8_t>((nanoseconds >> 56) & 0xFF);
	_data[8] = static_cast<uint8_t>(0x80 | (rand() & 0x3F));  
	
	for (size_t i = 9; i < 16; ++i) {
		_data[i] = static_cast<uint8_t>(rand() % 256);
	}
}

void UUID::_generateV4()
{
	// Generate a random UUID (version 4)

	static thread_local std::random_device rd;
	static thread_local std::mt19937 gen(rd());
	std::uniform_int_distribution<uint8_t> dis(0, 255);
	for (size_t i = 0; i < 16; ++i)
		_data[i] = dis(gen);

	// Set version to 4
	_data[6] = (_data[6] & 0x0F) | 0x40;
	
	// Set variant to RFC 4122
	_data[8] = (_data[8] & 0x3F) | 0x80;
}

bool UUID::parseFromString(const std::string &str)
{
	if (str.size() != 36)
		return false;


	if (str[8] != '-' || str[13] != '-' || str[18] != '-' || str[23] != '-')
		return false;

	_data.fill(0);

	size_t dataIndex = 0;
	for (size_t i = 0; i < str.size(); ++i) {
		if (str[i] == '-')
			continue;
	
		if (!isHexDigit(str[i]))
			return false; 
	
		if (dataIndex >= 32) 
			return false;

		uint8_t hexValue = hexCharToByte(str[i]);
		
		if (dataIndex % 2 == 0)
			_data[dataIndex / 2] = hexValue << 4;
		else 
			_data[dataIndex / 2] |= hexValue;
		
		dataIndex++;
	}
	
	return dataIndex == 32;
}

bool UUID::isHexDigit(char c) noexcept
{
	return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

uint8_t UUID::hexCharToByte(char c) noexcept
{
	if (c >= '0' && c <= '9')
		return c - '0';
	else if (c >= 'a' && c <= 'f') 
		return c - 'a' + 10;
	else if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	
	return 0;
}