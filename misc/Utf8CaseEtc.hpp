
    /** @brief Returns true if all characters in @a str are either
      *     letters or numbers.
      * @throws YstringException if str contains an invalid UTF-8 code point.
      */
    YSTRING_API bool isAlphaNumeric(std::string_view str);

    /** @brief Returns a lower case copy of @a str.
      */
    YSTRING_API std::string lower(std::string_view str);


    /** @brief Returns a title-cased copy of @a str.
      */
    YSTRING_API std::string title(std::string_view str);

    /** @brief Returns a upper case copy of @a str.
      */
    YSTRING_API std::string upper(std::string_view str);
