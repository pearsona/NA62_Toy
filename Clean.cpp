#include "sharedMemory/SharedMemoryManager.h"

int main(int argc, char *argv[]){
  na62::SharedMemoryManager::eraseAll();
  LOG_INFO("Cleaned Memory");
  return 1;
}
