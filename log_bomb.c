#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define BUFFER_SIZE 1024  // 1 KB

// Структура для передачи данных в поток
typedef struct {
    char *filename;
} ThreadData;

// Функция потока
void *zeroFirst1KB(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    FILE *file = fopen(data->filename, "r+");
    if (file == NULL) {
	    pthread_exit(NULL);
    }
    // Буфер 0
    char zeroBuffer[BUFFER_SIZE];
    memset(zeroBuffer, 0, BUFFER_SIZE);

    // Запись буфера в файл
    size_t bytesWritten = fwrite(zeroBuffer, 1, BUFFER_SIZE, file);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    
    // Передача данных в поток	
    int numFiles = argc - 1;
    pthread_t threads[numFiles];
    ThreadData threadData[numFiles];

    // Создание потока для каждого файла
    for (int i = 0; i < numFiles; i++) {
        threadData[i].filename = argv[i + 1];
	if (pthread_create(&threads[i], NULL, zeroFirst1KB,&threadData[i]) != 0) {
		return 1;
	}
    }
    // Ожидание конца всех потоков
    for (int i = 0; i < numFiles; i++) {
       if (pthread_join(threads[i], NULL) != 0) {
  		return 1;
       }
    }

    return 0;
}
