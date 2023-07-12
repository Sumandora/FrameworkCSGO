#ifndef SDK_GAMECLASS_PADDING
#define SDK_GAMECLASS_PADDING

#define CONCAT(a, b) a##b
#define PADDING_NAME(line) CONCAT(PAD, line)

#define PADDING(size)                       \
private:                                    \
	std::byte PADDING_NAME(__LINE__)[size]; \
                                            \
public:

#endif
