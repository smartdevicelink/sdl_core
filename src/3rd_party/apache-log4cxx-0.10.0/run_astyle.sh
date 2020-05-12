#!/bin/bash

astyle --recursive --style=java --indent=spaces --attach-namespaces --attach-classes --indent-classes --indent-switches --indent-namespaces --break-blocks --add-brackets src/main/include/\*.h src/main/cpp/\*.cpp
