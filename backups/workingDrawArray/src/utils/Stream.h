#pragma once
#include <string>
#include <fstream>
#include "../global.h"

namespace ul {
	UL_AN_NOTE("Move and streams ?")
	UL_AN_NOTE("Stream::write ?")
	UL_AN_NOTE("Modularity std::ios::trunc")

	/*
		Stream that allow reading or writting in a file
	*/
	template<typename _ChT>class Stream {
	public:
		using string_t = std::basic_string<_ChT>;

		/*
			Why is there 2 ops while 1 can do all the work ?
			We should optimize this so we use universal references to
			allow moving or not of strings and to avoid multiples overloads
			of operators taking rvalues references or lvalues.

			But integral types are faster to copy so we made a copy instead of moving them.

			Why use of enable_if ? The first op take an universal reference in argument so
			we must restrict types that can see this declaration otherwise all objets can
			be implicit cast to that so we can't overload this function. For the second one
			it's to respect the code modularity so we can add overloads to this without having
			the same problem of the first op.
		*/
		template<typename T, typename = std::enable_if_t<std::is_constructible_v<string_t, T>>>
		Stream& operator<<(T&&) = delete;

		template<typename _T, typename = std::enable_if_t<std::is_integral_v<_T>>>
		Stream& operator<<(_T) = delete;
		 

		Stream(const Stream&) = delete;
		Stream(Stream&&) = delete;
		Stream& operator=(const Stream&) = delete;
		Stream& operator=(Stream&&) = delete;
	protected:
		virtual void write(string_t) = 0;

		Stream() = default;
		~Stream() = default;
	};

	/*
		Stream file-safe, opening stream when write is needed and closing it
		after use. This stream should be used when multiples stream could write
		on the same file.
	*/
	template<typename _ChT>class OutSecureStream : public Stream<_ChT> {
	public:
		using string_t = std::basic_string<_ChT>;

		OutSecureStream(string_t name) : m_Filename(std::move(name)) {}
		OutSecureStream(const OutSecureStream&) = delete;
		OutSecureStream(OutSecureStream&&) = delete;
		~OutSecureStream() = default;
		OutSecureStream& operator=(const OutSecureStream&) = delete;
		OutSecureStream& operator=(OutSecureStream&&) = delete;

		template<typename T, typename = std::enable_if_t<std::is_constructible_v<string_t, T>>>
		Stream<_ChT>& operator<<(T&& s) {
			m_Stream.open(m_Filename);
			m_Stream << std::forward<T>(s);
			m_Stream.close();
			return *this;
		}

		template<typename _T, typename = std::enable_if_t<std::is_integral_v<_T>>>
		Stream<_ChT>& operator<<(_T i) {
			m_Stream.open(m_Filename);
			m_Stream << std::forward<_T>(i);
			m_Stream.close();
			return *this;
		}


	protected:
		virtual void write(string_t s) override final {}

	private:
		std::basic_ofstream<_ChT> m_Stream;
		string_t m_Filename;
	};

	/*
		Stream file-insafe opening stream at construction and closing it
		at destruction. This should be used on a file which is only written
		by this stream.
	*/
	template<typename _ChT>class OutInsecureStream : public Stream<_ChT> {
	public:
		using string_t = std::basic_string<_ChT>;

		OutInsecureStream(string_t name) : m_Filename(std::move(name)) { this->open(); }
		OutInsecureStream(const OutInsecureStream&) = delete;
		OutInsecureStream(OutInsecureStream&&) = delete;
		~OutInsecureStream() { this->close(); }
		OutInsecureStream& operator=(const OutInsecureStream&) = delete;
		OutInsecureStream& operator=(OutInsecureStream&&) = delete;

		template<typename T, typename = std::enable_if_t<std::is_constructible_v<string_t, T>>>
	    Stream<_ChT>& operator<<(T&& s) {
			m_Stream << s;
			return *this;
		}

		template<typename _T, typename = std::enable_if_t<std::is_integral_v<_T>>>
		Stream<_ChT>& operator<<(_T i) {
			m_Stream << i;
			return *this;
		}

	protected:
		virtual void write(string_t s) override final {}

	private:
		inline void open() {
			m_Stream.open(m_Filename, std::ios::trunc);
		}

		inline void close() {
			m_Stream.close();
		}

		std::basic_ofstream<_ChT> m_Stream;
		string_t m_Filename;
	};

}