#include "sharedMemory/SharedMemoryManager.h"

int main(int argc, char *argv[]){
  na62::SharedMemoryManager::eraseAll();
  LOG_INFO(" Cleaning Memory");
  return 1;
}
