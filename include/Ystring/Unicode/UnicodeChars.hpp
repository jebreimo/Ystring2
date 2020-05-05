//****************************************************************************
// Copyright © 2013 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2013-07-01
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <cstdint>

/** @file
  * @brief Defines constants for some common non-ASCII unicode code-points.
  */

namespace Ystring
{
    constexpr char32_t START_OF_HEADING = 0x0001u;
    constexpr char32_t START_OF_TEXT = 0x0002u;
    constexpr char32_t END_OF_TEXT = 0x0003u;
    constexpr char32_t END_OF_TRANSMISSION = 0x0004u;
    constexpr char32_t ENQUIRY = 0x0005u;
    constexpr char32_t ACKNOWLEDGE = 0x0006u;
    constexpr char32_t BELL = 0x0007u;
    constexpr char32_t BACKSPACE = 0x0008u;
    constexpr char32_t CHARACTER_TABULATION = 0x0009u;
    constexpr char32_t LINE_FEED = 0x000Au;
    constexpr char32_t LINE_TABULATION = 0x000Bu;
    constexpr char32_t FORM_FEED = 0x000Cu;
    constexpr char32_t CARRIAGE_RETURN = 0x000Du;
    constexpr char32_t SHIFT_OUT = 0x000Eu;
    constexpr char32_t SHIFT_IN = 0x000Fu;
    constexpr char32_t DATA_LINK_ESCAPE = 0x0010u;
    constexpr char32_t DEVICE_CONTROL_ONE = 0x0011u;
    constexpr char32_t DEVICE_CONTROL_TWO = 0x0012u;
    constexpr char32_t DEVICE_CONTROL_THREE = 0x0013u;
    constexpr char32_t DEVICE_CONTROL_FOUR = 0x0014u;
    constexpr char32_t NEGATIVE_ACKNOWLEDGE = 0x0015u;
    constexpr char32_t SYNCHRONOUS_IDLE = 0x0016u;
    constexpr char32_t END_OF_TRANSMISSION_BLOCK = 0x0017u;
    constexpr char32_t CANCEL = 0x0018u;
    constexpr char32_t END_OF_MEDIUM = 0x0019u;
    constexpr char32_t SUBSTITUTE = 0x001Au;
    constexpr char32_t ESCAPE = 0x001Bu;
    constexpr char32_t INFORMATION_SEPARATOR_FOUR = 0x001Cu;
    constexpr char32_t INFORMATION_SEPARATOR_THREE = 0x001Du;
    constexpr char32_t INFORMATION_SEPARATOR_TWO = 0x001Eu;
    constexpr char32_t INFORMATION_SEPARATOR_ONE = 0x001Fu;
    constexpr char32_t DELETE = 0x007Fu;
    constexpr char32_t BREAK_PERMITTED_HERE = 0x0082u;
    constexpr char32_t NO_BREAK_HERE = 0x0083u;
    constexpr char32_t NEXT_LINE = 0x0085u;
    constexpr char32_t START_OF_SELECTED_AREA = 0x0086u;
    constexpr char32_t END_OF_SELECTED_AREA = 0x0087u;
    constexpr char32_t CHARACTER_TABULATION_SET = 0x0088u;
    constexpr char32_t CHARACTER_TABULATION_WITH_JUSTIFICATION = 0x0089u;
    constexpr char32_t LINE_TABULATION_SET = 0x008Au;
    constexpr char32_t PARTIAL_LINE_FORWARD = 0x008Bu;
    constexpr char32_t PARTIAL_LINE_BACKWARD = 0x008Cu;
    constexpr char32_t REVERSE_LINE_FEED = 0x008Du;
    constexpr char32_t SINGLE_SHIFT_TWO = 0x008Eu;
    constexpr char32_t SINGLE_SHIFT_THREE = 0x008Fu;
    constexpr char32_t DEVICE_CONTROL_STRING = 0x0090u;
    constexpr char32_t PRIVATE_USE_ONE = 0x0091u;
    constexpr char32_t PRIVATE_USE_TWO = 0x0092u;
    constexpr char32_t SET_TRANSMIT_STATE = 0x0093u;
    constexpr char32_t CANCEL_CHARACTER = 0x0094u;
    constexpr char32_t MESSAGE_WAITING = 0x0095u;
    constexpr char32_t START_OF_GUARDED_AREA = 0x0096u;
    constexpr char32_t END_OF_GUARDED_AREA = 0x0097u;
    constexpr char32_t START_OF_STRING = 0x0098u;
    constexpr char32_t SINGLE_CHARACTER_INTRODUCER = 0x009Au;
    constexpr char32_t CONTROL_SEQUENCE_INTRODUCER = 0x009Bu;
    constexpr char32_t STRING_TERMINATOR = 0x009Cu;
    constexpr char32_t OPERATING_SYSTEM_COMMAND = 0x009Du;
    constexpr char32_t PRIVACY_MESSAGE = 0x009Eu;
    constexpr char32_t APPLICATION_PROGRAM_COMMAND = 0x009Fu;
    constexpr char32_t NO_BREAK_SPACE = 0x00A0u;
    constexpr char32_t INVERTED_EXCLAMATION_MARK = 0x00A1u;
    constexpr char32_t CENT_SIGN = 0x00A2u;
    constexpr char32_t POUND_SIGN = 0x00A3u;
    constexpr char32_t CURRENCY_SIGN = 0x00A4u;
    constexpr char32_t YEN_SIGN = 0x00A5u;
    constexpr char32_t BROKEN_BAR = 0x00A6u;
    constexpr char32_t SECTION_SIGN = 0x00A7u;
    constexpr char32_t DIAERESIS = 0x00A8u;
    constexpr char32_t COPYRIGHT_SIGN = 0x00A9u;
    constexpr char32_t FEMININE_ORDINAL_INDICATOR = 0x00AAu;
    constexpr char32_t LEFT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK = 0x00ABu;
    constexpr char32_t NOT_SIGN = 0x00ACu;
    constexpr char32_t SOFT_HYPHEN = 0x00ADu;
    constexpr char32_t REGISTERED_SIGN = 0x00AEu;
    constexpr char32_t MACRON = 0x00AFu;
    constexpr char32_t DEGREE_SIGN = 0x00B0u;
    constexpr char32_t PLUS_MINUS_SIGN = 0x00B1u;
    constexpr char32_t SUPERSCRIPT_TWO = 0x00B2u;
    constexpr char32_t SUPERSCRIPT_THREE = 0x00B3u;
    constexpr char32_t ACUTE_ACCENT = 0x00B4u;
    constexpr char32_t MICRO_SIGN = 0x00B5u;
    constexpr char32_t PILCROW_SIGN = 0x00B6u;
    constexpr char32_t MIDDLE_DOT = 0x00B7u;
    constexpr char32_t CEDILLA = 0x00B8u;
    constexpr char32_t SUPERSCRIPT_ONE = 0x00B9u;
    constexpr char32_t MASCULINE_ORDINAL_INDICATOR = 0x00BAu;
    constexpr char32_t RIGHT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK = 0x00BBu;
    constexpr char32_t VULGAR_FRACTION_ONE_QUARTER = 0x00BCu;
    constexpr char32_t VULGAR_FRACTION_ONE_HALF = 0x00BDu;
    constexpr char32_t VULGAR_FRACTION_THREE_QUARTERS = 0x00BEu;
    constexpr char32_t INVERTED_QUESTION_MARK = 0x00BFu;
    constexpr char32_t LATIN_CAPITAL_A_WITH_GRAVE = 0x00C0u;
    constexpr char32_t LATIN_CAPITAL_A_WITH_ACUTE = 0x00C1u;
    constexpr char32_t LATIN_CAPITAL_A_WITH_CIRCUMFLEX = 0x00C2u;
    constexpr char32_t LATIN_CAPITAL_A_WITH_TILDE = 0x00C3u;
    constexpr char32_t LATIN_CAPITAL_A_WITH_DIAERESIS = 0x00C4u;
    constexpr char32_t LATIN_CAPITAL_A_WITH_RING_ABOVE = 0x00C5u;
    constexpr char32_t LATIN_CAPITAL_AE = 0x00C6u;
    constexpr char32_t LATIN_CAPITAL_C_WITH_CEDILLA = 0x00C7u;
    constexpr char32_t LATIN_CAPITAL_E_WITH_GRAVE = 0x00C8u;
    constexpr char32_t LATIN_CAPITAL_E_WITH_ACUTE = 0x00C9u;
    constexpr char32_t LATIN_CAPITAL_E_WITH_CIRCUMFLEX = 0x00CAu;
    constexpr char32_t LATIN_CAPITAL_E_WITH_DIAERESIS = 0x00CBu;
    constexpr char32_t LATIN_CAPITAL_I_WITH_GRAVE = 0x00CCu;
    constexpr char32_t LATIN_CAPITAL_I_WITH_ACUTE = 0x00CDu;
    constexpr char32_t LATIN_CAPITAL_I_WITH_CIRCUMFLEX = 0x00CEu;
    constexpr char32_t LATIN_CAPITAL_I_WITH_DIAERESIS = 0x00CFu;
    constexpr char32_t LATIN_CAPITAL_ETH = 0x00D0u;
    constexpr char32_t LATIN_CAPITAL_N_WITH_TILDE = 0x00D1u;
    constexpr char32_t LATIN_CAPITAL_O_WITH_GRAVE = 0x00D2u;
    constexpr char32_t LATIN_CAPITAL_O_WITH_ACUTE = 0x00D3u;
    constexpr char32_t LATIN_CAPITAL_O_WITH_CIRCUMFLEX = 0x00D4u;
    constexpr char32_t LATIN_CAPITAL_O_WITH_TILDE = 0x00D5u;
    constexpr char32_t LATIN_CAPITAL_O_WITH_DIAERESIS = 0x00D6u;
    constexpr char32_t MULTIPLICATION_SIGN = 0x00D7u;
    constexpr char32_t LATIN_CAPITAL_O_WITH_STROKE = 0x00D8u;
    constexpr char32_t LATIN_CAPITAL_U_WITH_GRAVE = 0x00D9u;
    constexpr char32_t LATIN_CAPITAL_U_WITH_ACUTE = 0x00DAu;
    constexpr char32_t LATIN_CAPITAL_U_WITH_CIRCUMFLEX = 0x00DBu;
    constexpr char32_t LATIN_CAPITAL_U_WITH_DIAERESIS = 0x00DCu;
    constexpr char32_t LATIN_CAPITAL_Y_WITH_ACUTE = 0x00DDu;
    constexpr char32_t LATIN_CAPITAL_THORN = 0x00DEu;
    constexpr char32_t LATIN_SMALL_SHARP_S = 0x00DFu;
    constexpr char32_t LATIN_SMALL_A_WITH_GRAVE = 0x00E0u;
    constexpr char32_t LATIN_SMALL_A_WITH_ACUTE = 0x00E1u;
    constexpr char32_t LATIN_SMALL_A_WITH_CIRCUMFLEX = 0x00E2u;
    constexpr char32_t LATIN_SMALL_A_WITH_TILDE = 0x00E3u;
    constexpr char32_t LATIN_SMALL_A_WITH_DIAERESIS = 0x00E4u;
    constexpr char32_t LATIN_SMALL_A_WITH_RING_ABOVE = 0x00E5u;
    constexpr char32_t LATIN_SMALL_AE = 0x00E6u;
    constexpr char32_t LATIN_SMALL_C_WITH_CEDILLA = 0x00E7u;
    constexpr char32_t LATIN_SMALL_E_WITH_GRAVE = 0x00E8u;
    constexpr char32_t LATIN_SMALL_E_WITH_ACUTE = 0x00E9u;
    constexpr char32_t LATIN_SMALL_E_WITH_CIRCUMFLEX = 0x00EAu;
    constexpr char32_t LATIN_SMALL_E_WITH_DIAERESIS = 0x00EBu;
    constexpr char32_t LATIN_SMALL_I_WITH_GRAVE = 0x00ECu;
    constexpr char32_t LATIN_SMALL_I_WITH_ACUTE = 0x00EDu;
    constexpr char32_t LATIN_SMALL_I_WITH_CIRCUMFLEX = 0x00EEu;
    constexpr char32_t LATIN_SMALL_I_WITH_DIAERESIS = 0x00EFu;
    constexpr char32_t LATIN_SMALL_ETH = 0x00F0u;
    constexpr char32_t LATIN_SMALL_N_WITH_TILDE = 0x00F1u;
    constexpr char32_t LATIN_SMALL_O_WITH_GRAVE = 0x00F2u;
    constexpr char32_t LATIN_SMALL_O_WITH_ACUTE = 0x00F3u;
    constexpr char32_t LATIN_SMALL_O_WITH_CIRCUMFLEX = 0x00F4u;
    constexpr char32_t LATIN_SMALL_O_WITH_TILDE = 0x00F5u;
    constexpr char32_t LATIN_SMALL_O_WITH_DIAERESIS = 0x00F6u;
    constexpr char32_t DIVISION_SIGN = 0x00F7u;
    constexpr char32_t LATIN_SMALL_O_WITH_STROKE = 0x00F8u;
    constexpr char32_t LATIN_SMALL_U_WITH_GRAVE = 0x00F9u;
    constexpr char32_t LATIN_SMALL_U_WITH_ACUTE = 0x00FAu;
    constexpr char32_t LATIN_SMALL_U_WITH_CIRCUMFLEX = 0x00FBu;
    constexpr char32_t LATIN_SMALL_U_WITH_DIAERESIS = 0x00FCu;
    constexpr char32_t LATIN_SMALL_Y_WITH_ACUTE = 0x00FDu;
    constexpr char32_t LATIN_SMALL_THORN = 0x00FEu;
    constexpr char32_t LATIN_SMALL_Y_WITH_DIAERESIS = 0x00FFu;
    constexpr char32_t COMBINING_GRAVE_ACCENT = 0x0300u;
    constexpr char32_t COMBINING_ACUTE_ACCENT = 0x0301u;
    constexpr char32_t COMBINING_CIRCUMFLEX_ACCENT = 0x0302u;
    constexpr char32_t COMBINING_TILDE = 0x0303u;
    constexpr char32_t COMBINING_MACRON = 0x0304u;
    constexpr char32_t COMBINING_OVERLINE = 0x0305u;
    constexpr char32_t COMBINING_BREVE = 0x0306u;
    constexpr char32_t COMBINING_DOT_ABOVE = 0x0307u;
    constexpr char32_t COMBINING_DIAERESIS = 0x0308u;
    constexpr char32_t COMBINING_HOOK_ABOVE = 0x0309u;
    constexpr char32_t COMBINING_RING_ABOVE = 0x030Au;
    constexpr char32_t COMBINING_DOUBLE_ACUTE_ACCENT = 0x030Bu;
    constexpr char32_t COMBINING_CARON = 0x030Cu;
    constexpr char32_t COMBINING_VERTICAL_LINE_ABOVE = 0x030Du;
    constexpr char32_t COMBINING_DOUBLE_VERTICAL_LINE_ABOVE = 0x030Eu;
    constexpr char32_t COMBINING_DOUBLE_GRAVE_ACCENT = 0x030Fu;
    constexpr char32_t COMBINING_CANDRABINDU = 0x0310u;
    constexpr char32_t COMBINING_INVERTED_BREVE = 0x0311u;
    constexpr char32_t COMBINING_TURNED_COMMA_ABOVE = 0x0312u;
    constexpr char32_t COMBINING_COMMA_ABOVE = 0x0313u;
    constexpr char32_t COMBINING_REVERSED_COMMA_ABOVE = 0x0314u;
    constexpr char32_t COMBINING_COMMA_ABOVE_RIGHT = 0x0315u;
    constexpr char32_t COMBINING_GRAVE_ACCENT_BELOW = 0x0316u;
    constexpr char32_t COMBINING_ACUTE_ACCENT_BELOW = 0x0317u;
    constexpr char32_t COMBINING_LEFT_TACK_BELOW = 0x0318u;
    constexpr char32_t COMBINING_RIGHT_TACK_BELOW = 0x0319u;
    constexpr char32_t COMBINING_LEFT_ANGLE_ABOVE = 0x031Au;
    constexpr char32_t COMBINING_HORN = 0x031Bu;
    constexpr char32_t COMBINING_LEFT_HALF_RING_BELOW = 0x031Cu;
    constexpr char32_t COMBINING_UP_TACK_BELOW = 0x031Du;
    constexpr char32_t COMBINING_DOWN_TACK_BELOW = 0x031Eu;
    constexpr char32_t COMBINING_PLUS_SIGN_BELOW = 0x031Fu;
    constexpr char32_t COMBINING_MINUS_SIGN_BELOW = 0x0320u;
    constexpr char32_t COMBINING_PALATALIZED_HOOK_BELOW = 0x0321u;
    constexpr char32_t COMBINING_RETROFLEX_HOOK_BELOW = 0x0322u;
    constexpr char32_t COMBINING_DOT_BELOW = 0x0323u;
    constexpr char32_t COMBINING_DIAERESIS_BELOW = 0x0324u;
    constexpr char32_t COMBINING_RING_BELOW = 0x0325u;
    constexpr char32_t COMBINING_COMMA_BELOW = 0x0326u;
    constexpr char32_t COMBINING_CEDILLA = 0x0327u;
    constexpr char32_t COMBINING_OGONEK = 0x0328u;
    constexpr char32_t COMBINING_VERTICAL_LINE_BELOW = 0x0329u;
    constexpr char32_t COMBINING_BRIDGE_BELOW = 0x032Au;
    constexpr char32_t COMBINING_INVERTED_DOUBLE_ARCH_BELOW = 0x032Bu;
    constexpr char32_t COMBINING_CARON_BELOW = 0x032Cu;
    constexpr char32_t COMBINING_CIRCUMFLEX_ACCENT_BELOW = 0x032Du;
    constexpr char32_t COMBINING_BREVE_BELOW = 0x032Eu;
    constexpr char32_t COMBINING_INVERTED_BREVE_BELOW = 0x032Fu;
    constexpr char32_t COMBINING_TILDE_BELOW = 0x0330u;
    constexpr char32_t COMBINING_MACRON_BELOW = 0x0331u;
    constexpr char32_t COMBINING_LOW_LINE = 0x0332u;
    constexpr char32_t COMBINING_DOUBLE_LOW_LINE = 0x0333u;
    constexpr char32_t COMBINING_TILDE_OVERLAY = 0x0334u;
    constexpr char32_t COMBINING_SHORT_STROKE_OVERLAY = 0x0335u;
    constexpr char32_t COMBINING_LONG_STROKE_OVERLAY = 0x0336u;
    constexpr char32_t COMBINING_SHORT_SOLIDUS_OVERLAY = 0x0337u;
    constexpr char32_t COMBINING_LONG_SOLIDUS_OVERLAY = 0x0338u;
    constexpr char32_t COMBINING_RIGHT_HALF_RING_BELOW = 0x0339u;
    constexpr char32_t COMBINING_INVERTED_BRIDGE_BELOW = 0x033Au;
    constexpr char32_t COMBINING_SQUARE_BELOW = 0x033Bu;
    constexpr char32_t COMBINING_SEAGULL_BELOW = 0x033Cu;
    constexpr char32_t COMBINING_X_ABOVE = 0x033Du;
    constexpr char32_t COMBINING_VERTICAL_TILDE = 0x033Eu;
    constexpr char32_t COMBINING_DOUBLE_OVERLINE = 0x033Fu;
    constexpr char32_t COMBINING_GRAVE_TONE_MARK = 0x0340u;
    constexpr char32_t COMBINING_ACUTE_TONE_MARK = 0x0341u;
    constexpr char32_t COMBINING_GREEK_PERISPOMENI = 0x0342u;
    constexpr char32_t COMBINING_GREEK_KORONIS = 0x0343u;
    constexpr char32_t COMBINING_GREEK_DIALYTIKA_TONOS = 0x0344u;
    constexpr char32_t COMBINING_GREEK_YPOGEGRAMMENI = 0x0345u;
    constexpr char32_t COMBINING_BRIDGE_ABOVE = 0x0346u;
    constexpr char32_t COMBINING_EQUALS_SIGN_BELOW = 0x0347u;
    constexpr char32_t COMBINING_DOUBLE_VERTICAL_LINE_BELOW = 0x0348u;
    constexpr char32_t COMBINING_LEFT_ANGLE_BELOW = 0x0349u;
    constexpr char32_t COMBINING_NOT_TILDE_ABOVE = 0x034Au;
    constexpr char32_t COMBINING_HOMOTHETIC_ABOVE = 0x034Bu;
    constexpr char32_t COMBINING_ALMOST_EQUAL_TO_ABOVE = 0x034Cu;
    constexpr char32_t COMBINING_LEFT_RIGHT_ARROW_BELOW = 0x034Du;
    constexpr char32_t COMBINING_UPWARDS_ARROW_BELOW = 0x034Eu;
    constexpr char32_t COMBINING_GRAPHEME_JOINER = 0x034Fu;
    constexpr char32_t COMBINING_RIGHT_ARROWHEAD_ABOVE = 0x0350u;
    constexpr char32_t COMBINING_LEFT_HALF_RING_ABOVE = 0x0351u;
    constexpr char32_t COMBINING_FERMATA = 0x0352u;
    constexpr char32_t COMBINING_X_BELOW = 0x0353u;
    constexpr char32_t COMBINING_LEFT_ARROWHEAD_BELOW = 0x0354u;
    constexpr char32_t COMBINING_RIGHT_ARROWHEAD_BELOW = 0x0355u;
    constexpr char32_t COMBINING_RIGHT_ARROWHEAD_AND_UP_ARROWHEAD_BELOW = 0x0356u;
    constexpr char32_t COMBINING_RIGHT_HALF_RING_ABOVE = 0x0357u;
    constexpr char32_t COMBINING_DOT_ABOVE_RIGHT = 0x0358u;
    constexpr char32_t COMBINING_ASTERISK_BELOW = 0x0359u;
    constexpr char32_t COMBINING_DOUBLE_RING_BELOW = 0x035Au;
    constexpr char32_t COMBINING_ZIGZAG_ABOVE = 0x035Bu;
    constexpr char32_t COMBINING_DOUBLE_BREVE_BELOW = 0x035Cu;
    constexpr char32_t COMBINING_DOUBLE_BREVE = 0x035Du;
    constexpr char32_t COMBINING_DOUBLE_MACRON = 0x035Eu;
    constexpr char32_t COMBINING_DOUBLE_MACRON_BELOW = 0x035Fu;
    constexpr char32_t COMBINING_DOUBLE_TILDE = 0x0360u;
    constexpr char32_t COMBINING_DOUBLE_INVERTED_BREVE = 0x0361u;
    constexpr char32_t COMBINING_DOUBLE_RIGHTWARDS_ARROW_BELOW = 0x0362u;
    constexpr char32_t COMBINING_LATIN_SMALL_A = 0x0363u;
    constexpr char32_t COMBINING_LATIN_SMALL_E = 0x0364u;
    constexpr char32_t COMBINING_LATIN_SMALL_I = 0x0365u;
    constexpr char32_t COMBINING_LATIN_SMALL_O = 0x0366u;
    constexpr char32_t COMBINING_LATIN_SMALL_U = 0x0367u;
    constexpr char32_t COMBINING_LATIN_SMALL_C = 0x0368u;
    constexpr char32_t COMBINING_LATIN_SMALL_D = 0x0369u;
    constexpr char32_t COMBINING_LATIN_SMALL_H = 0x036Au;
    constexpr char32_t COMBINING_LATIN_SMALL_M = 0x036Bu;
    constexpr char32_t COMBINING_LATIN_SMALL_R = 0x036Cu;
    constexpr char32_t COMBINING_LATIN_SMALL_T = 0x036Du;
    constexpr char32_t COMBINING_LATIN_SMALL_V = 0x036Eu;
    constexpr char32_t COMBINING_LATIN_SMALL_X = 0x036Fu;
    constexpr char32_t GREEK_CAPITAL_ALPHA = 0x0391u;
    constexpr char32_t GREEK_CAPITAL_BETA = 0x0392u;
    constexpr char32_t GREEK_CAPITAL_GAMMA = 0x0393u;
    constexpr char32_t GREEK_CAPITAL_DELTA = 0x0394u;
    constexpr char32_t GREEK_CAPITAL_EPSILON = 0x0395u;
    constexpr char32_t GREEK_CAPITAL_ZETA = 0x0396u;
    constexpr char32_t GREEK_CAPITAL_ETA = 0x0397u;
    constexpr char32_t GREEK_CAPITAL_THETA = 0x0398u;
    constexpr char32_t GREEK_CAPITAL_IOTA = 0x0399u;
    constexpr char32_t GREEK_CAPITAL_KAPPA = 0x039Au;
    constexpr char32_t GREEK_CAPITAL_LAMDA = 0x039Bu;
    constexpr char32_t GREEK_CAPITAL_MU = 0x039Cu;
    constexpr char32_t GREEK_CAPITAL_NU = 0x039Du;
    constexpr char32_t GREEK_CAPITAL_XI = 0x039Eu;
    constexpr char32_t GREEK_CAPITAL_OMICRON = 0x039Fu;
    constexpr char32_t GREEK_CAPITAL_PI = 0x03A0u;
    constexpr char32_t GREEK_CAPITAL_RHO = 0x03A1u;
    constexpr char32_t GREEK_CAPITAL_SIGMA = 0x03A3u;
    constexpr char32_t GREEK_CAPITAL_TAU = 0x03A4u;
    constexpr char32_t GREEK_CAPITAL_UPSILON = 0x03A5u;
    constexpr char32_t GREEK_CAPITAL_PHI = 0x03A6u;
    constexpr char32_t GREEK_CAPITAL_CHI = 0x03A7u;
    constexpr char32_t GREEK_CAPITAL_PSI = 0x03A8u;
    constexpr char32_t GREEK_CAPITAL_OMEGA = 0x03A9u;
    constexpr char32_t GREEK_SMALL_ALPHA = 0x03B1u;
    constexpr char32_t GREEK_SMALL_BETA = 0x03B2u;
    constexpr char32_t GREEK_SMALL_GAMMA = 0x03B3u;
    constexpr char32_t GREEK_SMALL_DELTA = 0x03B4u;
    constexpr char32_t GREEK_SMALL_EPSILON = 0x03B5u;
    constexpr char32_t GREEK_SMALL_ZETA = 0x03B6u;
    constexpr char32_t GREEK_SMALL_ETA = 0x03B7u;
    constexpr char32_t GREEK_SMALL_THETA = 0x03B8u;
    constexpr char32_t GREEK_SMALL_IOTA = 0x03B9u;
    constexpr char32_t GREEK_SMALL_KAPPA = 0x03BAu;
    constexpr char32_t GREEK_SMALL_LAMDA = 0x03BBu;
    constexpr char32_t GREEK_SMALL_MU = 0x03BCu;
    constexpr char32_t GREEK_SMALL_NU = 0x03BDu;
    constexpr char32_t GREEK_SMALL_XI = 0x03BEu;
    constexpr char32_t GREEK_SMALL_OMICRON = 0x03BFu;
    constexpr char32_t GREEK_SMALL_PI = 0x03C0u;
    constexpr char32_t GREEK_SMALL_RHO = 0x03C1u;
    constexpr char32_t GREEK_SMALL_FINAL_SIGMA = 0x03C2u;
    constexpr char32_t GREEK_SMALL_SIGMA = 0x03C3u;
    constexpr char32_t GREEK_SMALL_TAU = 0x03C4u;
    constexpr char32_t GREEK_SMALL_UPSILON = 0x03C5u;
    constexpr char32_t GREEK_SMALL_PHI = 0x03C6u;
    constexpr char32_t GREEK_SMALL_CHI = 0x03C7u;
    constexpr char32_t GREEK_SMALL_PSI = 0x03C8u;
    constexpr char32_t GREEK_SMALL_OMEGA = 0x03C9u;
    constexpr char32_t OGHAM_SPACE_MARK = 0x1680u;
    constexpr char32_t MONGOLIAN_VOWEL_SEPARATOR = 0x180Eu;
    constexpr char32_t EN_QUAD = 0x2000u;
    constexpr char32_t EM_QUAD = 0x2001u;
    constexpr char32_t EN_SPACE = 0x2002u;
    constexpr char32_t EM_SPACE = 0x2003u;
    constexpr char32_t THREE_PER_EM_SPACE = 0x2004u;
    constexpr char32_t FOUR_PER_EM_SPACE = 0x2005u;
    constexpr char32_t SIX_PER_EM_SPACE = 0x2006u;
    constexpr char32_t FIGURE_SPACE = 0x2007u;
    constexpr char32_t PUNCTUATION_SPACE = 0x2008u;
    constexpr char32_t THIN_SPACE = 0x2009u;
    constexpr char32_t HAIR_SPACE = 0x200Au;
    constexpr char32_t ZERO_WIDTH_SPACE = 0x200Bu;
    constexpr char32_t ZERO_WIDTH_NON_JOINER = 0x200Cu;
    constexpr char32_t ZERO_WIDTH_JOINER = 0x200Du;
    constexpr char32_t LINE_SEPARATOR = 0x2028u;
    constexpr char32_t PARAGRAPH_SEPARATOR = 0x2029u;
    constexpr char32_t NARROW_NO_BREAK_SPACE = 0x202Fu;
    constexpr char32_t MEDIUM_MATHEMATICAL_SPACE = 0x205Fu;
    constexpr char32_t WORD_JOINER = 0x2060u;
    constexpr char32_t IDEOGRAPHIC_SPACE = 0x3000u;
    constexpr char32_t ZERO_WIDTH_NO_BREAK_SPACE = 0xFEFFu;
    constexpr char32_t REPLACEMENT_CHARACTER = 0xFFFDu;
    constexpr char32_t INVALID = 0xFFFFFFFFu;
}
