#!/usr/bin/perl

use strict;

# read in data
open(FILE, "<file");
my @buffer = <FILE>;
close(FILE);

my $line;
my %FORM;
my $name;
my $value;
foreach $line (@buffer)
{
	($name, $value) = split(/=>/, $line);
	$FORM{$name} = $value;
}

# generate table

my $html = <<"END HTML";
Content-Type: text/html

<!DOCTYPE html>
<html>
<html>
  <table border="1">
    <tr>
      <td>Name</td>
      <td>Email</td>
      <td>Invitation code</td>
      <td>Password</td>
    </tr>
	<tr>
	  <td>$FORM{'user_name'}</td>
      <td>$FORM{'user_email'}</td>
      <td>$FORM{'user_code'}</td>
      <td>$FORM{'user_passwd'}</td>
    </tr>
  </table>
</html>

END HTML

print $html;