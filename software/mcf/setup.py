# MCF SetupScript
from distutils.core import setup
import py2exe

# Run the build process by entering 'setup.py py2exe' or
# 'python setup.py py2exe' in a console prompt.


setup(
    # The first three parameters are not required, if at least a
    # 'version' is given, then a versioninfo resource is built from
    # them and added to the executables.
    version = "1.0.0",
    description = "MagicConfig Command Line Utility",
    name = "MCF",

    # targets to build
    console = ["MCF.py"],
    )
