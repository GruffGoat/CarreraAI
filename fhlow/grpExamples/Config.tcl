#-----------------------------------------------------------------------------------
#Copyright (C) 2005 Christian Kitzler, Simon Lasselsberger, Markus Pfaff
# christian.kitzler@fh-hagenberg.at
# simon.lasselsberger@fh-hagenberg.at
# markus.pfaff@fh-hagenberg.at
#
#This file is part of the fhlow - scripting environment Library.
#
#The fhlow - scripting environment Library is free software; you can redistribute it
#and/or modify it under the terms of the GNU General Public License
#as published by the Free Software Foundation; either version 2 of the
#License, or (at your option) any later version.
#
#fhlow - scripting environment is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with fhlow - scripting environment; if not, write to the Free Software
#Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#-----------------------------------------------------------------------------------

####################################################################################
#                                                                                  #
#    you can find more documentation about FHLOW at http://esther.lisuna.org/      #
#                                                                                  #
####################################################################################


#***********************************************************************************
#*************  General Stuff  *****************************************************
#***********************************************************************************


############## Packages ##################################################
# purpose: declares the packages which are needed by this unit
# 
# syntax:  
#          append Packages {
#              {SomeGroup SomePackage}
#              {AnotherGroup AnotherPackage}
#          }
#          
#          -- SomeGroup:   name of the group which contains your packages
#          -- SomePackage: name of the package your unit needs
#
# misc:    DO NOT ADD "grp" OR "pkg" TO YOUR GROUP/PACKAGE NAME!!!
#          
##########################################################################
append Packages {
}

