TEMPLATE = subdirs

SUBDIRS = library library-tests basic-console-project towers-optimization-project
library-tests.depends = library
basic-console-project.depends = library
towers-optmization-project.depends = library
