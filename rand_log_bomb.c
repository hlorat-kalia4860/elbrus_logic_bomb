#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

// Структура для передачи данных в поток
typedef struct {
    const char *filename; // Имя файла
    long num_values;       // Количество случайных значений
    unsigned int seed;     // Уникальное начальное значение для генератора случайных чисел
} ThreadData;

// Функция, которая будет выполняться в каждом потоке
void *fill_file_with_binary(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    FILE *file;

    // Открываем файл для записи в текстовом режиме
    file = fopen(data->filename, "r+");
    if (file == NULL) {
        perror("Failed to open file");
        pthread_exit(NULL);
    }

    // Инициализируем генератор случайных чисел для этого потока
    srand(data->seed);

    // Записываем случайные бинарные значения (0 или 1) в файл
    for (long i = 0; i < data->num_values; i++) {
        int random_value = rand() % 2; // Генерация случайного числа 0 или 1
        fprintf(file, "%d", random_value); // Запись числа в файл
    }

    // Закрываем файл
    fclose(file);

    //printf("File '%s' filled with %ld random binary values (seed: %u).\n", data->filename, data->num_values, data->seed);
    pthread_exit(NULL);
}

// Функция для получения текущего времени в микросекундах
unsigned int get_seed() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (unsigned int)(tv.tv_sec * 1000000 + tv.tv_usec);
}

int main(int argc, char *argv[]) {
    // Проверяем, что переданы имена файлов
    if (argc < 2) {
        printf("Usage: %s <file1> <file2> ... <fileN>\n", argv[0]);
        return 1;
    }

    const long num_values = 1024; // Количество случайных бинарных значений для каждого файла
    int num_files = argc - 1;     // Количество файлов
    pthread_t threads[num_files]; // Массив идентификаторов потоков
    ThreadData thread_data[num_files]; // Массив данных для потоков

    // Создаем потоки для обработки каждого файла
    for (int i = 0; i < num_files; i++) {
        thread_data[i].filename = argv[i + 1];
        thread_data[i].num_values = num_values;
        thread_data[i].seed = get_seed(); // Уникальное начальное значение для каждого потока

        // Создаем поток
        if (pthread_create(&threads[i], NULL, fill_file_with_binary, &thread_data[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    // Ожидаем завершения всех потоков
    for (int i = 0; i < num_files; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Failed to join thread");
            return 1;
        }
    }

    return 0;
}
