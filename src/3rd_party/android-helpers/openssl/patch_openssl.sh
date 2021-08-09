#!/bin/bash \

##### Prepare Files #####
sed -i 's/.*-mandroid.*//' Configurations/15-android.conf
patch -p1 -N <<EOP
--- old/Configurations/unix-Makefile.tmpl   2018-09-11 14:48:19.000000000 +0200
+++ new/Configurations/unix-Makefile.tmpl   2018-10-18 09:06:27.282007245 +0200
@@ -43,12 +43,17 @@
      # will return the name from shlib(\$libname) with any SO version number
      # removed.  On some systems, they may therefore return the exact same
      # string.
-     sub shlib {
+     sub shlib_simple {
          my \$lib = shift;
          return () if \$disabled{shared} || \$lib =~ /\\.a$/;
-         return \$unified_info{sharednames}->{\$lib}. \$shlibvariant. '\$(SHLIB_EXT)';
+
+         if (windowsdll()) {
+             return \$lib . '\$(SHLIB_EXT_IMPORT)';
+         }
+         return \$lib .  '\$(SHLIB_EXT_SIMPLE)';
      }
-     sub shlib_simple {
+     
+   sub shlib {
          my \$lib = shift;
          return () if \$disabled{shared} || \$lib =~ /\\.a$/;

EOP