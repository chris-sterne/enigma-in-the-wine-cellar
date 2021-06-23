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

# All Blender .ply format mesh files are converted into custom .msh
# format files that can be efficiently loaded as an Linux OS resource.
# The maximum number of vertices is 65536 (a warning will be printed if
# the total exceeds this maximum). 
#
# The binary (little-endian byte) format of the .msh files is as follows:
#
# File identifier (4-byte "msh\n")
# Total vertices (4-byte integer)
# Array of vertex coordinates (X, Y, Z) (3 x 4-byte floats)
# Array of vertex colours (R, G, B, A) (4 x 4-byte floats) 

my $SourceDirectoryName = "Blender";
my $DestinationDirectoryName = "Meshes";

# Delete all binary mesh files in the destination directory.

print `rm ./$DestinationDirectoryName/*.msh`;

# Open source directory of ASCII mesh files. 

opendir( my $SourceDirectoryHandle, $SourceDirectoryName )
  or die "Could not open directory: ", $SourceDirectoryName;

# Process each file that has a ".ply" extension, which are 
# Polygon (Stanford) File Format containing ascii values.

while ( my $FileName = readdir( $SourceDirectoryHandle ))
{
  next unless ( -f "$SourceDirectoryName/$FileName" );
  next unless ( $FileName =~ /\.ply/ );

  print "Processing ASCII image mesh file: $FileName \n";

  open( my $SourceFileHandle, '<:encoding(UTF-8)', "$SourceDirectoryName/$FileName" ) 
    or die "Could not open image mesh file for reading: ", $FileName;

  # Examine each row of the image mesh file header.

  my @elements = ();
  my @totals = ();

  my @parts;
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

  my @coordinates = ();
  my @colours  = ();
  my @indices  = ();

  my $index = 0;

  foreach ( @elements )
  {
    if ( $_ =~ /vertex/ )
    {
      while ( $totals[ $index ] > 0 ) 
      {
        if ( defined( my $row = <$SourceFileHandle> ))
        {
          @parts = split( /\s+/, $row );

          # Store vertex coordinates X,Y,Z colours R,G,B,A.
          # The normals X,Y,Z are ignored.  The Alpha channel
          # value is set to maximum.

          push @coordinates, $parts[ 0 ], $parts[ 1 ], $parts[ 2 ];
          push @colours, $parts[ 6 ], $parts[ 7 ], $parts[ 8 ], 255;
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

          # Store face vertex indices.

	  push @indices, $parts[ 1 ], $parts[ 2 ], $parts[ 3 ];

          # Display a warning if the face uses more than three indices.

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

  # Close the source image mesh file.

  close( $SourceFileHandle );

  # Open a binary mesh file for writing.  The first part of this file's name
  # will be the same as the source file and will have a '.msh' extension.
  # e.g. BlockWall_A.ply -> BlockWall_A.msh

  $FileName =~ s/.ply/.msh/;

  open( my $DestinationFileHandle, '>', "$DestinationDirectoryName/$FileName" )
    or die "Could not open binary mesh file for writing: ", $FileName;

  # Write the binary mesh file header.

  print $DestinationFileHandle "msh\n";
  print $DestinationFileHandle pack( 'L<', scalar @indices );

  # Write the array of vertex coordinates.

  foreach ( @indices )
  {
    $index = $_ * 3;

    print $DestinationFileHandle pack( 'f<', $coordinates[$index + 0] );
    print $DestinationFileHandle pack( 'f<', $coordinates[$index + 1] ); 
    print $DestinationFileHandle pack( 'f<', $coordinates[$index + 2] ); 
  }	  

  # Write the array of vertex colours.

  foreach ( @indices )
  {
    $index = $_ * 4;

    print $DestinationFileHandle pack( 'f<', $colours[$index + 0] / 255.0 );
    print $DestinationFileHandle pack( 'f<', $colours[$index + 1] / 255.0 ); 
    print $DestinationFileHandle pack( 'f<', $colours[$index + 2] / 255.0 );
    print $DestinationFileHandle pack( 'f<', $colours[$index + 3] / 255.0 ); 
  }

  # Display a warning if the total number of faces exceeds a 2-byte value
  # (the face vertex indices are only 2-byte values).

  if ( scalar @coordinates > 65535*3 )
  {
    print "*** WARNING *** Mesh contains more than 65536 vertices.\n";
  }

  # Close the destination image mesh file.

  close( $DestinationFileHandle );
}

# Close source image mesh directory.

close( $SourceDirectoryHandle );

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

# Process each file that has a ".msh" extension.

while ( my $FileName = readdir( $DirectoryHandle ))
{
  next unless ( -f "$DirectoryName/$FileName" );
  next unless ( $FileName =~ /\.msh/ );

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

# Open directory of shader files. 

$DirectoryName = 'Shaders';

opendir( $DirectoryHandle, $DirectoryName )
  or die "Could not open directory: ", $DirectoryName;

# Process each file that has a ".glsl" extension.

while ( my $FileName = readdir( $DirectoryHandle ))
{
  next unless ( -f "$DirectoryName/$FileName" );
  next unless ( $FileName =~ /\.glsl/ );

  # Add line with resource name to XML file.

  print "Adding shader file name: $FileName \n";
  print( $XMLHandle "    \<file\>Shaders/$FileName\</file\>\n" );
}

# Close shader directory.

close( $DirectoryHandle );

# Write the trailer lines of the XML block.

print( $XMLHandle "  \</gresource\>\n" );
print( $XMLHandle "\</gresources\>\n" );

close( $XMLHandle );



