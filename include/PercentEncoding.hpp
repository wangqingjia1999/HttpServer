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

		std::string Encode(const std::string& s);

		std::string Decode(const std::string& s);

		bool isFinished();

		char getDecodedeCharacter();

	private:
		struct Impl;
		std::unique_ptr< Impl > impl_;
	};
}