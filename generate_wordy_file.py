from RandomWordGenerator import RandomWord

# TODO add argument list [filename; words_to_write]
WORDS_TO_WRITE = 100_000_000
WORDS_TO_REQUEST = 10_000


def convert_list_to_str(lst):
    return ' '.join(lst)


with open("wordy_file.txt", "w+") as my_file:
    rw = RandomWord(max_word_size=10,
                    constant_word_size=False,
                    include_digits=False)

    for i in range(int(WORDS_TO_WRITE / WORDS_TO_REQUEST)):
        word_list = rw.getList(num_of_words=WORDS_TO_REQUEST)
        my_file.write(convert_list_to_str(word_list) + ' ')
