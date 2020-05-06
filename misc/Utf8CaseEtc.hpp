
    /** @brief Compares @a str and @a cmp, ignoring any differences in
      *     letter casing.
      *
      * @note Composed and decomposed versions of the same characters are
      *     treated as different characters (the decomposed character is
      *     typically the "lesser" one).
      * @returns @arg < 0 if @a str is less than @a cmp
      *          @arg 0 if @a str is equal to @a cmp
      *          @arg > 0 if @a str is greater than @a cmp
      * @throw YstringException if str contains an invalid UTF-8 code point.
      */
    YSTRING_API int32_t caseInsensitiveCompare(std::string_view str,
                                               std::string_view cmp);

    /** @brief Returns true if the upper case versions of @a str and @a cmp
      *     are equal.
      *
      * @note Composed and decomposed versions of the same characters are
      *     treated as different characters (the decomposed character is
      *     typically the "lesser" one).
      * @throw YstringException if str contains an invalid UTF-16 code point.
      */
    YSTRING_API bool caseInsensitiveEqual(std::string_view str,
                                          std::string_view cmp);

    /** @brief Returns true if the upper case version of @a str is less
      *     than @a cmp.
      *
      * Only a quick comparison of code point values are performed. This
      * function should not be used to sort strings in alphabetical order as
      * what is alphabetical order varies between languages and cultures.
      * @throw YstringException if str contains an invalid UTF-8 code point.
      */
    YSTRING_API bool caseInsensitiveLess(std::string_view str,
                                         std::string_view cmp);

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
