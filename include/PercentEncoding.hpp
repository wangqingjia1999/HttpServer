#pragma once
#include <string>
#include <memory>

namespace Uri
{
	class PercentEncoding
	{
		// Lifecycle management
	public:
		~PercentEncoding();
		PercentEncoding();
		PercentEncoding(const PercentEncoding&) noexcept;
		PercentEncoding(PercentEncoding&&) noexcept;
		PercentEncoding& operator=(const PercentEncoding&) noexcept;
		PercentEncoding& operator=(PercentEncoding&&) noexcept;

		// Member methods
	public:

		std::string encode(const std::string& s);

		std::string decode(const std::string& s);

		bool is_finished();

		char get_decoded_character();

	private:
		struct Impl;
		std::unique_ptr< Impl > impl_;
	};
}