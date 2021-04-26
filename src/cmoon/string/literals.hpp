#pragma once

#define STR_LITERALS(...) __VA_ARGS__, L##__VA_ARGS__, u8##__VA_ARGS__, u##__VA_ARGS__, U##__VA_ARGS__
#define CHAR_LITERALS(...) STR_LITERALS(__VA_ARGS__)