#include "Compressor.hpp"

#include <iostream>

namespace Compressor
{
	std::string compress(const std::string& data_string)
	{
		z_stream stream = {0};

		if (deflateInit(&stream, Z_BEST_COMPRESSION) != Z_OK)
			return {};

		stream.next_in = (Bytef*)(data_string.data());
		stream.avail_in = data_string.size();

		std::string compressed_data;
		const size_t DATA_SIZE = data_string.size();
		char output_buffer[DATA_SIZE + 1];

		int result = 0;
		do
		{
			stream.next_out = reinterpret_cast<Bytef*>(output_buffer);
			stream.avail_out = sizeof(output_buffer);

			result = deflate(&stream, Z_FINISH);

			if (compressed_data.size() < stream.total_out)
				compressed_data.append(
				    output_buffer, stream.total_out - compressed_data.size());

		} while (result == Z_OK);

		deflateEnd(&stream);

		if (result != Z_STREAM_END)
			return {};

		return compressed_data;
	}
} // namespace Compressor
