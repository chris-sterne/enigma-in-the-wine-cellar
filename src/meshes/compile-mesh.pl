#!/usr/local/bin/perl

#---------------------------------------------------------------------------
# Enigma In The Wine Cellar
# April 7, 2021
#---------------------------------------------------------------------------
# This PERL script converts an image mesh file in Stanford Polygon format
# (.ply extension) into a custom binary mesh (.msh extension) resource file.
# The input filename prefix is copied to the output filename.
#
# usage:  perl compile-mesh [filename.ply]
# result: filename.msh resource file
#---------------------------------------------------------------------------

use strict;
use warnings;

# Get command line argument count.

my $total_args = $#ARGV + 1;

# Print script usage if no arguments are provided.

if ($total_args == 0)
{
	print "usage: perl compile-mesh [filename.ply]\n";
	exit;
}

# A filename.ply format mesh file is converted into custom filename.msh
# format file that can be efficiently loaded as a Linux OS resource.
# The maximum number of vertices is 65536 (a warning will be printed if
# the total exceeds this maximum). 
#
# The binary (little-endian) format of the .msh files is as follows:
#
# File identifier (4-byte "msh\n")
# Total vertices (4-byte integer)
# Array of vertex coordinates (X, Y, Z) (3 x 4-byte floats)
# Array of vertex colours (R, G, B, A) (4 x 4-byte floats)

my $filename = $ARGV[0];

open(my $input_filehandle, '<:encoding(UTF-8)', "$filename") 
	or die "Could not open input mesh file for reading: ", $filename;

# Examine each row of the image mesh file header.

my @elements = ();
my @totals = ();

my @parts;
my $ascii = 0;

while (my $row = <$input_filehandle>)
{
	@parts = split( /\s+/, $row );

	if ($parts[0] =~ /format/)
	{
		# Set ascii flag if value format is ascii.

		if ($parts[1] =~ /ascii/)
		{
			$ascii = 1;
		}
	}
	elsif ($parts[0] =~ /element/)
	{
		# Save the number of vertices and faces.

		if ($parts[1] =~ /vertex/)
		{
			push(@elements, $parts[1]);
			push(@totals, $parts[2]);
		}
		elsif ($parts[1] =~ /face/)
		{ 
			push(@elements, $parts[1]);
			push(@totals, $parts[2]);
		}
	}
	elsif ($parts[0] =~ /end_header/)
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

foreach (@elements)
{
	if ($_ =~ /vertex/)
	{
		while ($totals[$index] > 0)
		{
			if (defined(my $row = <$input_filehandle>))
			{
				@parts = split(/\s+/, $row);

				# Store vertex coordinates X,Y,Z colours R,G,B,A.
				# The normals X,Y,Z are ignored.  The Alpha channel
				# value is set to maximum.

				push @coordinates, $parts[0], $parts[1], $parts[2];
				push @colours, $parts[6], $parts[7], $parts[8], 255;
			}

			$totals[$index] --;
			}
		}
		elsif ($_ =~ /face/)
		{
		while ($totals[$index] > 0) 
		{
			if ( defined(my $row = <$input_filehandle>))
			{
				@parts = split( /\s+/, $row );

				# Store face vertex indices.

				push @indices, $parts[1], $parts[2], $parts[3];

				# Display a warning if the face uses more than three indices.

				if ($parts[0] != 3)
				{
					print "*** WARNING *** Mesh contains non-triangular face.\n";
				}
			}

			$totals[$index] --;
		}
	}

	$index ++;
}

# Close the input mesh file.

close($input_filehandle);

# Open an output mesh file for writing.  The output filename will be the same
# as the input filename, but with the '.ply' extension replaced with '.msh'.
#
# e.g. BlockWall_A.ply -> BlockWall_A.msh

$filename =~ s/.ply/.msh/;

open(my $output_filehandle, '>', "$filename")
	or die "Could not open output mesh file for writing: ", $filename;

# Write the mesh file header.

print $output_filehandle "msh\n";
print $output_filehandle pack('L<', scalar @indices);

# Write the array of vertex coordinates.

foreach (@indices)
{
	$index = $_ * 3;

	print $output_filehandle pack('f<', $coordinates[$index + 0]);
	print $output_filehandle pack('f<', $coordinates[$index + 1]); 
	print $output_filehandle pack('f<', $coordinates[$index + 2]); 
}	  

# Write the array of vertex colours.

foreach (@indices)
{
	$index = $_ * 4;

	print $output_filehandle pack('f<', $colours[$index + 0] / 255.0);
	print $output_filehandle pack('f<', $colours[$index + 1] / 255.0); 
	print $output_filehandle pack('f<', $colours[$index + 2] / 255.0);
	print $output_filehandle pack('f<', $colours[$index + 3] / 255.0); 
}

# Display a warning if the total number of faces exceeds a 2-byte value
# (the face vertex indices are only 2-byte values).

if (scalar @coordinates > (65535 * 3))
{
	print "*** WARNING *** Mesh contains more than 65536 vertices.\n";
}

# Close the output mesh file.

close($output_filehandle);


