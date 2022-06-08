#!/usr/bin/perl
use strict;
use CGI qw/:standard/;

#打开相应的html来写入
open(FILE, "<$ARGV[0]");
my @buffer = <FILE>;
close(FILE);

if($ARGV[1] == '0'){
	open(FILE, "+>filetest.pl");
	print FILE '#!/usr/bin/perl'."\n";
}
else{
	open(FILE, ">>filetest.pl");
}

print FILE 'my $html = <<"END HTML";'."\n";
print FILE 'Content-Type: text/html'."\n\n";
my $line;
foreach $line (@buffer)
{
	print FILE $line;
}
print FILE 'END HTML'."\n";
print FILE 'print $html;';
close(FILE);

print redirect ('/cgi-bin/filetest.pl');