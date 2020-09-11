#ifndef SP_LOG_ENGINE_PROVIDER_HEADER
#define SP_LOG_ENGINE_PROVIDER_HEADER

#include "SpectrumFronend.h"
#include "SpLogger.h"

namespace NAMESPACE_FRONTEND
{
#define MAX_LINES 100
#define MAX_CHARS 256
	class SpLogEngineProvider 
		: public SpLogProvider
	{
	private:
		sp_char _messages[MAX_LINES][MAX_CHARS];
		sp_uint _length;
		sp_uint _next;
		sp_uint _column;

		void addMessage(const sp_char* text)
		{
			if (_column != ZERO_UINT)
			{
				sp_uint len = strlen(text);

				if (_column + len > MAX_CHARS)
				{
					len = MAX_CHARS - _column;
					std::memcpy(&_messages[_next - ONE_UINT][_column], text, len);
					_column = ZERO_UINT;
				}
				else
				{
					std::memcpy(&_messages[_next - ONE_UINT][_column], text, len);
					_column += len;
				}

				return;
			}

			if (_length != MAX_LINES)
				_length++;
			else
				if (_next == MAX_LINES)
					_next = ZERO_UINT;

			_column = strlen(text);
			std::memcpy(_messages[_next], text, _column);
			_next++;
		}
		
	public:

		API_INTERFACE SpLogEngineProvider()
		{
			_length = ZERO_UINT;
			_next = ZERO_UINT;
			_column = ZERO_UINT;
		}

		API_INTERFACE void error(const sp_char* text) override
		{
			addMessage(text);
		}

		API_INTERFACE virtual void info(const sp_char* text) override
		{
			addMessage(text);
		}

		API_INTERFACE virtual void debug(const sp_char* text) override
		{
			addMessage(text);
		}
		
		API_INTERFACE virtual void newLine() override
		{
			_messages[_next - ONE_UINT][_column] = END_OF_STRING;
			_column = ZERO_UINT;
		}

		API_INTERFACE inline sp_uint messageLength() const
		{
			return _length;
		}

		API_INTERFACE inline sp_char* message(const sp_uint index)
		{
			if (_length == MAX_LINES)
			{
				const sp_uint i = index + _next;

				if (i > _length)
					return _messages[i - _length - ONE_UINT];
				else
					return _messages[i - ONE_UINT];
			}

			return _messages[index];
		}

		API_INTERFACE inline void clear()
		{
			_next = ZERO_UINT;
			_length = ZERO_UINT;
		}

	};
#undef MAX_CHARS
#undef MAX_LINES
}

#endif // SP_LOG_ENGINE_PROVIDER_HEADER