#!/usr/local/bin/perl

#--------------------------------------------------#
# Enigma In The Wine Cellar                        #
# Version 3.0 for Linux OS                         #
# March 10, 2016                                   #
#--------------------------------------------------#
# This PERL script creates a source and header     #
# file containing all game resources.  These files #
# would then be compiled with the game source code #
# to embed these resources into the final          #
# executable.                                      #
#--------------------------------------------------#

use strict;
use warnings;

#---------------------------------------------------------#
#          ASCII to BINARY Mesh File Conversion           #
#---------------------------------------------------------#

# The following table contains the number of bytes used for data types
# in binary PLY mesh files.

# name        type        number of bytes
# ---------------------------------------
# char       character                 1
# uchar      unsigned character        1
# short      short integer             2
# ushort     unsigned short integer    2
# int        integer                   4
# uint       unsigned integer          4
# float      single-precision float    4
# double     double-precision float    8

# The Source directory contains ascii image mesh files.
# These will be converted into binary (big-endian) image mesh files
# in the destination directory.  The binary image mesh file will have
# the same name as the ascii image mesh file.

my $SourceDirectoryName      = "Blender";
my $DestinationDirectoryName = "Meshes";

# Delete all binary image mesh files in the destination directory.

print `rm ./$DestinationDirectoryName/*`;

# Open directory containing ascii image mesh files. 

opendir( my $SourceDirectoryHandle, $SourceDirectoryName )
  or die "Could not open directory: ", $SourceDirectoryName;

# Process each file that has a ".ply" extension, which are 
# Polygon (Stanford) File Format containing ascii values.

while ( my $FileName = readdir( $SourceDirectoryHandle ))
{
  next unless ( -f "$SourceDirectoryName/$FileName" );
  next unless ( $FileName =~ /\.ply/ );

  print "Processing ascii image mesh file: $FileName \n";

  open( my $SourceFileHandle, '<:encoding(UTF-8)', "$SourceDirectoryName/$FileName" ) 
    or die "Could not open image mesh file for reading: ", $FileName;

  # Examine each row of the image mesh file header.

  my @parts;
  my @elements;
  my @totals;
  my $ascii = 0;

  while (my $row = <$SourceFileHandle>)
  {
    @parts = split( /\s+/, $row );

    if ( $parts[0] =~ /format/ )
    {
      # Set ascii flag if value format is ascii.

      if ( $parts[1] =~ /ascii/ )
      {
        $ascii = 1;
      }
    }
    elsif ( $parts[0] =~ /element/ )
    {
      # Save the number of vertices and faces.

      if ( $parts[1] =~ /vertex/ )
      {
        push( @elements, $parts[1] );
        push( @totals, $parts[2] );
      }
      elsif ( $parts[1] =~ /face/ )
      { 
        push( @elements, $parts[1] );
        push( @totals, $parts[2] );
      }
    }
    elsif ( $parts[0] =~ /end_header/ )
    {
      # Stop reading the header when its end has been found.
      # The mesh data follows the header.

      last;
    }
  }

  # Open a binary mesh file for writing.  This file will have the same name
  # as the source file, but will be in a different directory.

  open( my $DestinationFileHandle, '>', "$DestinationDirectoryName/$FileName" )
    or die "Could not open binary mesh file for writing: ", $FileName;

  # Write the binary mesh file header.

  print $DestinationFileHandle "ply\n";
  print $DestinationFileHandle "comment Enigma in the Wine Cellar 3 object mesh\n";
  print $DestinationFileHandle "format binary_big_endian 1.0\n";

  my $index = 0;

  foreach ( @elements )
  {
    if ( $_ =~ /vertex/ )
    {
      print $DestinationFileHandle "element vertex $totals[ $index ]\n";
      print $DestinationFileHandle "property float x\n";
      print $DestinationFileHandle "property float y\n";
      print $DestinationFileHandle "property float z\n";
      print $DestinationFileHandle "property uchar red\n";
      print $DestinationFileHandle "property uchar green\n";
      print $DestinationFileHandle "property uchar blue\n";
    }
    elsif ( $_ =~ /face/ )
    {
      print $DestinationFileHandle "element face $totals[ $index ]\n";
      print $DestinationFileHandle "property list uchar ushort vertex_indices\n";

      # Display a warning if the mesh contains more vertices than can be accessed
      # by the 16-bit indices in the face elements.

      if ( $totals[ $index ] > 65536 )
      {
        print "*** WARNING *** Mesh contains more than 65536 vertices.\n";
      }
    }

    $index ++;
  }

  print $DestinationFileHandle "end_header\n";

  # Convert all ascii data into binary.

  $index = 0;

  foreach ( @elements )
  {
    if ( $_ =~ /vertex/ )
    {
      while ( $totals[ $index ] > 0 ) 
      {
        if ( defined( my $row = <$SourceFileHandle> ))
        {
          @parts = split( /\s+/, $row );

          # Write vertex coordinate X,Y,Z as single-precision 32-bit floats in big-endian format. 

          print $DestinationFileHandle pack( 'f>', $parts[ 0 ] );
          print $DestinationFileHandle pack( 'f>', $parts[ 1 ] );
          print $DestinationFileHandle pack( 'f>', $parts[ 2 ] );

          # Write vertex color R,G,B as unsigned 8-bit characters.

          print $DestinationFileHandle pack( 'C', $parts[6] );
          print $DestinationFileHandle pack( 'C', $parts[7] );
          print $DestinationFileHandle pack( 'C', $parts[8] );
        }

        $totals[ $index ] --;
      }
    }
    elsif ( $_ =~ /face/ )
    {
      while ( $totals[ $index ] > 0 ) 
      {
        if ( defined( my $row = <$SourceFileHandle> ))
        {
          @parts = split( /\s+/, $row );

          # Write face vertex count as an unsigned 8-bit character.

          print $DestinationFileHandle pack( 'C', $parts[0] );

          # Write face vertex indices as unsigned 16-bit integers in bit-endian format.

          print $DestinationFileHandle pack( 'S>', $parts[1] );
          print $DestinationFileHandle pack( 'S>', $parts[2] );
          print $DestinationFileHandle pack( 'S>', $parts[3] );

          # Display a warning if the face has more than 3 vertices.

          if ( $parts[0] != 3 )
          {
            print "*** WARNING *** Mesh contains non-triangular face.\n";
          }
        }

        $totals[ $index ] --;
      }
    }

    $index ++;  
  }

  # Close the source and destination image mesh files.

  close( $SourceFileHandle );
  close( $DestinationFileHandle );
}

#---------------------------------------------------------#
#                Resource Integration                     #
#---------------------------------------------------------#

# Open GResources.xml XML markup file that will list
# all resource files to be combined into source code.

my $XMLName = 'EnigmaWC.gresource.xml';

open( my $XMLHandle, '>:encoding(UTF-8)', $XMLName )
  or die "Could not open file: ", $XMLName;

# Write the header lines of the XML block.

print( $XMLHandle "\<?xml version=\"1.0\" encoding=\"UTF-8\"?\>\n" );
print( $XMLHandle "\<gresources\>\n" );
print( $XMLHandle "  \<gresource prefix=\"/org/game/EnigmaWC\"\>\n" );

# Open directory of image mesh files. 

my $DirectoryName = 'Meshes';

opendir( my $DirectoryHandle, $DirectoryName )
  or die "Could not open directory: ", $DirectoryName;

# Process each file that has a ".ply" extension.

while ( my $FileName = readdir( $DirectoryHandle ))
{
  next unless ( -f "$DirectoryName/$FileName" );
  next unless ( $FileName =~ /\.ply/ );

  # Add line with resource name to XML file.

  print "Adding image mesh file name: $FileName \n";
  print( $XMLHandle "    \<file\>$DirectoryName/$FileName\</file\>\n" );
}

# Close image mesh directory.

close( $DirectoryHandle );

# Open directory of image bitmap files. 

$DirectoryName = 'Bitmaps';

opendir( $DirectoryHandle, $DirectoryName )
  or die "Could not open directory: ", $DirectoryName;

# Process each file that has a ".png" extension.

while ( my $FileName = readdir( $DirectoryHandle ))
{
  next unless ( -f "$DirectoryName/$FileName" );
  next unless ( $FileName =~ /\.png/ );

  # Add line with resource name to XML file.

  print "Adding bitmap file name: $FileName \n";
  print( $XMLHandle "    \<file\>Bitmaps/$FileName\</file\>\n" );
}

# Close image bitmap directory.

close( $DirectoryHandle );

# Write the trailer lines of the XML block.

print( $XMLHandle "  \</gresource\>\n" );
print( $XMLHandle "\</gresources\>\n" );

close( $XMLHandle );

# Create source and header files.

`glib-compile-resources --generate-source --target=../source/EnigmaWC.gresource.cpp $XMLName`;
`glib-compile-resources --generate-header --target=../include/EnigmaWC.gresource.h $XMLName`;



