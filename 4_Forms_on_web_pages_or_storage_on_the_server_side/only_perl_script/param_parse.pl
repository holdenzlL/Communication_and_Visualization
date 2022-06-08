#!/usr/bin/perl

use strict;
use CGI qw/:standard/;

$ENV{'REQUEST_METHOD'} =~ tr/a-z/A-Z/;

my $buffer;
if ($ENV{'REQUEST_METHOD'} eq "POST")
{
   read(STDIN, $buffer, $ENV{'CONTENT_LENGTH'});
}else {
   $buffer = $ENV{'QUERY_STRING'};
}
my @pairs = split(/&/, $buffer);

my $pair;
my $name;
my $value;
my %FORM;
open(FILE, "+>file");
foreach $pair (@pairs)
{
   ($name, $value) = split(/=/, $pair);
   $value =~ tr/+/ /;
   $value =~ s/%(..)/pack("C", hex($1))/eg;
   $FORM{$name} = $value;
   print FILE "$name=>$value\n";
}
close(FILE);

if($FORM{'user_code'} ne 'K3852'){
	# 如果不相等的话，那么直接输出失败的html
	my @args = ("html_generator.pl", "response_n.html", '0');
	system($^X, @args);
}
else{
	#如果相等的话，那么先输出头，再输出html
	open(FILE, "+>filetest.pl");
	print FILE '#!/usr/bin/perl'."\n";
	print FILE 'my $Username = \''."$FORM{'user_name'}"."'"."\n".';';
	print FILE 'my $invitation_code = \''."$FORM{'user_code'}"."'"."\n".';';
	print FILE 'my $Email = \''."$FORM{'user_email'}"."'"."\n".';';
	print FILE 'my $MY_email = \''.'test@test.de'."'"."\n".';';
	close(FILE);
	my @args = ("html_generator.pl", "response_y.html", '1');
	system($^X, @args);
}