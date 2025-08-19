/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe_iostream.tpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 00:01:34 by lagea             #+#    #+#             */
/*   Updated: 2025/08/19 14:09:54 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREAD_SAFE_IOSTREAM_TPP
# define THREAD_SAFE_IOSTREAM_TPP

template<typename T>
ThreadSafeIOStream &ThreadSafeIOStream::operator<<(const T &value)
{
	std::ostringstream oss;
	oss << value;
	std::string text = oss.str();
	
	_buffer += text;

	return *this;
}

template<typename T>
ThreadSafeIOStream &ThreadSafeIOStream::operator>>(T &value) 
{
	std::lock_guard<std::mutex> lock(_global_mutex);
	
	print_prefix_if_needed();
	std::cin >> value;
	_line_start = true;
	
	return *this;
}

template<typename T>
void ThreadSafeIOStream::prompt(const std::string &question, T &dest) 
{
	std::lock_guard<std::mutex> lock(_global_mutex);
	
	std::string formatted_question = format_with_prefix(question + " ");
	std::cout << formatted_question;
	std::cout.flush();
	
	std::cin >> dest;
	_line_start = true;
}

#endif
