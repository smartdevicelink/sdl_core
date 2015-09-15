#!/usr/bin/env bash

echo "installing brew"
ruby -e "$(curl -fsSL https://raw.github.com/Homebrew/homebrew/go/install)"

echo "installing git"
brew install git

echo "updating brew"
brew update

echo "installing cmake"
brew install cmake

echo "installing log4cxx"
brew install log4cxx

