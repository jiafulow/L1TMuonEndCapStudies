#!/usr/bin/env python

"""
Based on CERN CRAB3 UserTarball.py and CMSSWConfig.py
"""

import os
import glob
import tarfile

class UserTarball(object):
    def __init__(self, name='default.tgz', mode='w:gz', dereference=True):
        self.name = name
        self.mode = mode
        self.dereference = dereference
        self.sendPythonFolder = True
        self.sendExternalFolder = False
        self.scriptExe = None

        self.directories = ['lib', 'biglib', 'module', 'bin']
        #if self.sendPythonFolder:
        #    self.directories += ['python', 'cfipython']
        if self.sendExternalFolder:
            self.directories += ['external']

        # Note that dataDirs are only looked-for and added under the src/ folder.
        # /data/ subdirs contain data files needed by the code
        # /interface/ subdirs contain C++ header files needed e.g. by ROOT6
        self.dataDirs = ['data', 'interface']
        if self.sendPythonFolder:
            self.dataDirs += ['python', 'cfipython']

    def add_files(self, userFiles=None, excludeFiles=None):
        """
        Add the necessary files to the tarball
        """
        userFiles = userFiles or []
        excludeFiles = excludeFiles or []

        def filter_fn(tarinfo):
            #print tarinfo.name
            if os.path.split(tarinfo.name)[1] in excludeFiles:
                return None
            else:
                return tarinfo

        with tarfile.open(name=self.name, mode=self.mode, dereference=self.dereference) as tar:
            # Tar up whole directories
            for directory in self.directories:
                fullPath = os.path.join(os.environ['CMSSW_BASE'], directory)
                if os.path.exists(fullPath):
                    tar.add(fullPath, directory, recursive=True, filter=filter_fn)

            # Search for and tar up "data" directories in src/
            srcPath = os.path.join(os.environ['CMSSW_BASE'], 'src')
            for root, dirs, files in os.walk(srcPath):
                if os.path.basename(root) in self.dataDirs:
                    directory = root.replace(srcPath, 'src')
                    tar.add(root, directory, recursive=True, filter=filter_fn)

            # Tar up extra files the user needs
            for globName in userFiles:
                fileNames = glob.glob(globName)
                if not fileNames:
                    raise Exception("The input file '%s' cannot be found." % globName)
                for filename in fileNames:
                    directory = os.path.basename(filename)
                    tar.add(filename, directory, recursive=True, filter=filter_fn)

            if self.scriptExe:
                tar.add(self.scriptExe, arcname=os.path.basename(self.scriptExe))


# ______________________________________________________________________________
if __name__ == '__main__':

    print('[INFO   ] Packing tarball ...')
    print('[INFO   ] Using CMSSW_BASE: %s' % (os.environ['CMSSW_BASE']))
    print('[INFO   ] Using CMSSW_VERSION: %s' % (os.environ['CMSSW_VERSION']))
    tb = UserTarball()
    tb.add_files(userFiles=['pset.py', 'pset_imp.py'], excludeFiles=['LUT_AndrewFix_25July16.dat','LUT_v5_24Oct16.dat'])
    print('[INFO   ] %s%s is created (%iM).%s' % ('\033[92m', tb.name, os.stat(tb.name).st_size >> 20, '\033[0m'))
