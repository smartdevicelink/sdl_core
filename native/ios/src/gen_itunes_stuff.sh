#!/bin/bash

USAGE=$( cat <<EOF
Usage: ${0} base_url

base_url is the URL where the ipa and generated plist and index.html files can
be downloaded from when published.

This script is supposed to run after a build on a CI server.
EOF
)

base_url=${1?"${USAGE}"}
git_version=$(git log -1 --format="%h")
bundleversion="$( /usr/libexec/PlistBuddy -c 'Print :CFBundleVersion' native/ios/src/avatar/avatar-Info.plist )"
appname="MFT Guide"

i=0

# for each build* subdirectory
for dir in build*; do
    # extract suffix
    suffix="${dir:5}"
    # extract language, or "en" if null
    lang="${dir:6}"
    lang="${lang:-en}"

    # add git commit hash to ipa's filename
    cd "$dir"
    cur_ipa_file=$( ls -1 *.ipa )
    ipa_file="MFT_Guide${suffix}_${git_version}.ipa"
    mv "$cur_ipa_file" "$ipa_file"

    # generate plist file
    bundleid="com.luxoft.avatar${suffix}"
    bundletitle="${appname} ${lang} ${bundleversion} (${git_version})"
    ipa_url="${base_url}${ipa_file}"
    plist_file="avatar${suffix}.plist"
    full_plist_url="${base_url}${plist_file}"

    full_plist_urls[i]="$full_plist_url"
    bundletitles[i]="$bundletitle"

    cat >"${plist_file}" <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
	<key>items</key>
	<array>
		<dict>
			<key>assets</key>
			<array>
				<dict>
					<key>kind</key>
					<string>software-package</string>
					<key>url</key>
					<string>${ipa_url}</string>
				</dict>
			</array>
			<key>metadata</key>
			<dict>
				<key>bundle-identifier</key>
				<string>${bundleid}</string>
				<key>bundle-version</key>
				<string>${bundleversion}</string>
				<key>kind</key>
				<string>software</string>
				<key>title</key>
				<string>${bundletitle}</string>
			</dict>
		</dict>
	</array>
</dict>
</plist>
EOF

    let "i += 1"
    cd ..
done


cat >index.html <<EOF
<!DOCTYPE html>
<html>
<head>
<title>Install ${appname}</title>
<meta name="viewport" id="viewport" content="width=320, user-scalable=no, minimum-scale=1, maximum-scale=1" />
<style>
body {
    font-family: "Helvetica";
    padding: 10px;
}
</style>
</head>
<body>
<ul>
EOF

for i in $(seq 0 $((${#full_plist_urls[@]} - 1))); do
    cat >>index.html <<EOF
    <li><a href="itms-services://?action=download-manifest&url=${full_plist_urls[i]}">Install ${bundletitles[i]}</a></li>
EOF
done

cat >>index.html <<EOF
</ul>
</body>
</html>
EOF

