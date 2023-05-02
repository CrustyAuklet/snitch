#ifndef SNITCH_MACROS_CONSTEXPR_HPP
#define SNITCH_MACROS_CONSTEXPR_HPP

#include "snitch/snitch_config.hpp"
#include "snitch/snitch_expression.hpp"
#include "snitch/snitch_macros_check_base.hpp"
#include "snitch/snitch_macros_warnings.hpp"
#include "snitch/snitch_matcher.hpp"
#include "snitch/snitch_registry.hpp"
#include "snitch/snitch_test_data.hpp"

#define SNITCH_CONSTEXPR_REQUIRE_IMPL(CHECK, EXPECTED, MAYBE_ABORT, ...)                           \
    do {                                                                                           \
        auto& SNITCH_CURRENT_TEST = snitch::impl::get_current_test();                              \
        SNITCH_WARNING_PUSH                                                                        \
        SNITCH_WARNING_DISABLE_PARENTHESES                                                         \
        SNITCH_WARNING_DISABLE_CONSTANT_COMPARISON                                                 \
        if constexpr (SNITCH_IS_DECOMPOSABLE(__VA_ARGS__)) {                                       \
            {                                                                                      \
                constexpr SNITCH_EXPR(CHECK "[compile-time]", EXPECTED, __VA_ARGS__);              \
                SNITCH_CURRENT_TEST.reg.report_assertion(                                          \
                    SNITCH_CURRENT_EXPRESSION.success, SNITCH_CURRENT_TEST, {__FILE__, __LINE__},  \
                    SNITCH_CURRENT_EXPRESSION);                                                    \
                if (!SNITCH_CURRENT_EXPRESSION.success) {                                          \
                    MAYBE_ABORT;                                                                   \
                }                                                                                  \
            }                                                                                      \
            {                                                                                      \
                SNITCH_EXPR(CHECK "[run-time]", EXPECTED, __VA_ARGS__);                            \
                SNITCH_CURRENT_TEST.reg.report_assertion(                                          \
                    SNITCH_CURRENT_EXPRESSION.success, SNITCH_CURRENT_TEST, {__FILE__, __LINE__},  \
                    SNITCH_CURRENT_EXPRESSION);                                                    \
                if (!SNITCH_CURRENT_EXPRESSION.success) {                                          \
                    MAYBE_ABORT;                                                                   \
                }                                                                                  \
            }                                                                                      \
        } else {                                                                                   \
            {                                                                                      \
                constexpr bool SNITCH_TEMP_RESULT = static_cast<bool>(__VA_ARGS__);                \
                SNITCH_CURRENT_TEST.reg.report_assertion(                                          \
                    SNITCH_TEMP_RESULT == EXPECTED, SNITCH_CURRENT_TEST, {__FILE__, __LINE__},     \
                    CHECK "[compile-time](" #__VA_ARGS__ ")");                                     \
                if (SNITCH_TEMP_RESULT != EXPECTED) {                                              \
                    MAYBE_ABORT;                                                                   \
                }                                                                                  \
            }                                                                                      \
            {                                                                                      \
                const bool SNITCH_TEMP_RESULT = static_cast<bool>(__VA_ARGS__);                    \
                SNITCH_CURRENT_TEST.reg.report_assertion(                                          \
                    SNITCH_TEMP_RESULT == EXPECTED, SNITCH_CURRENT_TEST, {__FILE__, __LINE__},     \
                    CHECK "[run-time](" #__VA_ARGS__ ")");                                         \
                if (SNITCH_TEMP_RESULT != EXPECTED) {                                              \
                    MAYBE_ABORT;                                                                   \
                }                                                                                  \
            }                                                                                      \
        }                                                                                          \
        SNITCH_WARNING_POP                                                                         \
    } while (0)

// clang-format off
#define SNITCH_CONSTEXPR_REQUIRE(...)       SNITCH_CONSTEXPR_REQUIRE_IMPL("CONSTEXPR_REQUIRE",       true,  SNITCH_TESTING_ABORT,  __VA_ARGS__)
#define SNITCH_CONSTEXPR_CHECK(...)         SNITCH_CONSTEXPR_REQUIRE_IMPL("CONSTEXPR_CHECK",         true,  (void)0,               __VA_ARGS__)
#define SNITCH_CONSTEXPR_REQUIRE_FALSE(...) SNITCH_CONSTEXPR_REQUIRE_IMPL("CONSTEXPR_REQUIRE_FALSE", false, SNITCH_TESTING_ABORT,  __VA_ARGS__)
#define SNITCH_CONSTEXPR_CHECK_FALSE(...)   SNITCH_CONSTEXPR_REQUIRE_IMPL("CONSTEXPR_CHECK_FALSE",   false, (void)0,               __VA_ARGS__)
// clang-format on

#define SNITCH_CONSTEXPR_REQUIRE_THAT_IMPL(CHECK, MAYBE_ABORT, EXPR, ...)                          \
    do {                                                                                           \
        auto& SNITCH_CURRENT_TEST = snitch::impl::get_current_test();                              \
        {                                                                                          \
            constexpr auto SNITCH_TEMP_RESULT = snitch::impl::match(EXPR, __VA_ARGS__);            \
            SNITCH_CURRENT_TEST.reg.report_assertion(                                              \
                SNITCH_TEMP_RESULT.first, SNITCH_CURRENT_TEST, {__FILE__, __LINE__},               \
                CHECK "[compile-time](" #EXPR ", " #__VA_ARGS__ "), got ",                         \
                SNITCH_TEMP_RESULT.second);                                                        \
            if (!SNITCH_TEMP_RESULT.first) {                                                       \
                MAYBE_ABORT;                                                                       \
            }                                                                                      \
        }                                                                                          \
        {                                                                                          \
            const auto SNITCH_TEMP_RESULT = snitch::impl::match(EXPR, __VA_ARGS__);                \
            SNITCH_CURRENT_TEST.reg.report_assertion(                                              \
                SNITCH_TEMP_RESULT.first, SNITCH_CURRENT_TEST, {__FILE__, __LINE__},               \
                CHECK "[run-time](" #EXPR ", " #__VA_ARGS__ "), got ", SNITCH_TEMP_RESULT.second); \
            if (!SNITCH_TEMP_RESULT.first) {                                                       \
                MAYBE_ABORT;                                                                       \
            }                                                                                      \
        }                                                                                          \
    } while (0)

// clang-format off
#define SNITCH_CONSTEXPR_REQUIRE_THAT(EXPR, ...) SNITCH_CONSTEXPR_REQUIRE_THAT_IMPL("CONSTEXPR_REQUIRE_THAT", SNITCH_TESTING_ABORT,  EXPR, __VA_ARGS__)
#define SNITCH_CONSTEXPR_CHECK_THAT(EXPR, ...)   SNITCH_CONSTEXPR_REQUIRE_THAT_IMPL("CONSTEXPR_CHECK_THAT",   (void)0,               EXPR, __VA_ARGS__)
// clang-format on

// clang-format off
#if SNITCH_WITH_SHORTHAND_MACROS
#    define CONSTEXPR_REQUIRE(...)           SNITCH_CONSTEXPR_REQUIRE(__VA_ARGS__)
#    define CONSTEXPR_CHECK(...)             SNITCH_CONSTEXPR_CHECK(__VA_ARGS__)
#    define CONSTEXPR_REQUIRE_FALSE(...)     SNITCH_CONSTEXPR_REQUIRE_FALSE(__VA_ARGS__)
#    define CONSTEXPR_CHECK_FALSE(...)       SNITCH_CONSTEXPR_CHECK_FALSE(__VA_ARGS__)
#    define CONSTEXPR_REQUIRE_THAT(EXP, ...) SNITCH_CONSTEXPR_REQUIRE_THAT(EXP, __VA_ARGS__)
#    define CONSTEXPR_CHECK_THAT(EXP, ...)   SNITCH_CONSTEXPR_CHECK_THAT(EXP, __VA_ARGS__)
#endif
// clang-format on

#endif