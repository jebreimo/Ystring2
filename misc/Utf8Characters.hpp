
    /** @brief Returns an iterator to the start of character number @a n
      *     starting at @a first.
      */
    YSTRING_API std::string::iterator nextCharacter(
            std::string::iterator& first,
            std::string::iterator& last,
            size_t n = 1);

    /** @brief Returns an iterator to the start of character number @a n
      *     starting at @a first.
      */
    YSTRING_API std::string::const_iterator nextCharacter(
            std::string::const_iterator& first,
            std::string::const_iterator& last,
            size_t n = 1);

    /** @brief Returns an iterator to the start of character number @a n
      *     from the start of string @a str.
      * @param n The number of complete characters (i.e. not bytes, not
      *     even code points if the string has decomposed characters) from the
      *     start of the string. If @a pos is negative it's from the end of
      *     the string instead.
      */
    YSTRING_API std::string::iterator nthCharacter(
            std::string& str,
            ptrdiff_t n);

    /** @brief Returns an iterator to the start of character number @a n
      *     from the start of string @a str.
      * @param n The number of complete characters (i.e. not bytes, not
      *     even code points if the string has decomposed characters) from the
      *     start of the string. If @a pos is negative it's from the end of
      *     the string instead.
      */
    YSTRING_API std::string::const_iterator nthCharacter(
            std::string_view str,
            ptrdiff_t n);

    /** @brief Returns an iterator to the start of character number @a n
      *     counting backwards from @a last.
      */
    YSTRING_API std::string::iterator prevCharacter(
            std::string::iterator& first,
            std::string::iterator& last,
            size_t n = 1);

    /** @brief Returns an iterator to the start of character number @a n
      *     counting backwards from @a last.
      */
    YSTRING_API std::string::const_iterator prevCharacter(
            std::string::const_iterator& first,
            std::string::const_iterator& last,
            size_t n = 1);

    /** @brief Returns the number of characters in @a str.
      *
      * @note A composed character can consist of multiple code points.
      * @return the number of characters.
      * @throw YstringException if str contains an invalid UTF-8 code point.
      */
    YSTRING_API size_t countCharacters(std::string_view str);
