
    /** @brief Returns true if @a str ends with @a cmp.
      * @note Composed and decomposed versions of the same characters are
      *     treated as different characters.
      */
    YSTRING_API bool endsWith(std::string_view str,
                              std::string_view cmp,
                              FindFlags_t flags = FindFlags::DEFAULTS);

    /** @brief Returns the first substring in @a str that matches @a cmp.
      * @note Composed and decomposed versions of the same characters are
      *     treated as different characters.
      * @return A pair of iterators where first points to the start and
      *     second points to the end of the substring within @a str.
      *     If the substring can't be found both point to @a str.end().
      */
    YSTRING_API std::string_view findFirst(
            std::string_view str,
            std::string_view cmp,
            FindFlags_t flags = FindFlags::DEFAULTS);

    /** @brief Returns the last substring in @a str that matches @a cmp.
      * @note Composed and decomposed versions of the same characters are
      *     treated as different characters.
      * @return A pair of iterators where first points to the start and
      *     second points to the end of the substring within @a str.
      *     If the substring can't be found both point to @a str.begin().
      */
    YSTRING_API std::string_view findLast(
            std::string_view str,
            std::string_view cmp,
            FindFlags_t flags = FindFlags::DEFAULTS);

    /** @brief Returns true if all characters in @a str are either
      *     letters or numbers.
      * @throws YstringException if str contains an invalid UTF-8 code point.
      */
    YSTRING_API bool isAlphaNumeric(std::string_view str);

    /** @brief Returns a lower case copy of @a str.
      */
    YSTRING_API std::string lower(std::string_view str);

    /** @brief Returns a copy of @a str where instances of @a from are
      *     replaced with @a to.
      *
      * @param maxReplacements The maximum number of replacements that will be
      *     performed. All instances of @a from are replaced if the
      *     value is 0. If it is negative at most abs(maxReplacements) will be
      *     made, starting at the end of the string.
      */
    YSTRING_API std::string replace(
            std::string_view str,
            std::string_view from,
            std::string_view to,
            ptrdiff_t maxReplacements = 0,
            FindFlags_t flags = FindFlags::DEFAULTS);

    /** @brief Returns true if @a str starts with substring @a cmp.
      * @throw YstringException if @a str or @a cmp contain any invalid UTF-8
      *     code points.
      */
    YSTRING_API bool startsWith(std::string_view str,
                                std::string_view cmp,
                                FindFlags_t flags = FindFlags::DEFAULTS);

    /** @brief Returns a title-cased copy of @a str.
      */
    YSTRING_API std::string title(std::string_view str);

    /** @brief Returns a upper case copy of @a str.
      */
    YSTRING_API std::string upper(std::string_view str);
