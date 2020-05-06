
    /** @brief Returns a copy of @a str where control character etc. have
      *     been escaped.
      *
      * When escaping with backslashes the function does not use octal codes,
      * not even \\0 for value 0 as these are too easy to misinterpret.
      */
    YSTRING_API std::string escape(
            std::string_view str,
            EscapeType_t type = EscapeType::BACKSLASH);

    /** @brief Returns a copy of @a str where non-UTF-8 bytes have
      *     been escaped with a \\x and the bytes hexadecimal value.
      */
    YSTRING_API std::string escapeInvalidUtf8(
            std::string_view str);

    /** @brief Returns true if @a str has characters that will be unescaped
      *     if unescape is called with the same parameters.
      */
    YSTRING_API bool hasEscapedCharacters(
            std::string_view str,
            EscapeType_t type = EscapeType::BACKSLASH);

    /** @brief Returns true if @a str has characters that will be escaped
      *     if escape is called with the same parameters.
      */
    YSTRING_API bool hasUnescapedCharacters(
            std::string_view str,
            EscapeType_t type = EscapeType::BACKSLASH);

    /** @brief Returns a copy of @a str where all escape sequences have been
      *     translated to the characters they represent.
      * @throws YstringException if @a str contains an invalid
      *     escape sequence.
      */
    YSTRING_API std::string unescape(
            std::string_view str,
            EscapeType_t type = EscapeType::BACKSLASH);
