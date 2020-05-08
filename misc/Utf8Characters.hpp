
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
