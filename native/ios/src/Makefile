
default:
	# Set default make action here
	# xcodebuild -target Tests -configuration MyMainTarget -sdk iphonesimulator build

clean:
	#-rm -rf build/*
	xcodebuild -scheme test -workspace avatar.xcworkspace -configuration Debug -sdk iphonesimulator clean

test:
	#GHUNIT_CLI=1 xcodebuild -target Tests -configuration Debug -sdk iphonesimulator build
	GHUNIT_CLI=1 xcodebuild -scheme test -workspace avatar.xcworkspace -configuration Debug -sdk iphonesimulator build

.PHONY: test clean

