#!/usr/bin/perl
 
use strict;
 
use Fcntl qw( :DEFAULT :flock :seek );
use File::Spec;
use IO::Handle;
 
die "Usage: $0 startdir search replace\n"
    unless scalar @ARGV == 3;
my $startdir = shift @ARGV || '.';
my $search = shift @ARGV or
    die "Search parameter cannot be empty.\n";
my $replace = shift @ARGV;
$search = qr/\Q$search\E/o;
 
my @stack;
 
sub process_file($) {
    my $file = shift;
    my $fh = new IO::Handle;
    sysopen $fh, $file, O_RDONLY or
        die "Cannot read $file: $!\n";
    my $found;
    while(my $line = <$fh>) {
        if($line =~ /$search/) {
            $found = 1;
            last;
        }
    }
    if($found) {
        print "  Processing in $file\n";
        seek $fh, 0, SEEK_SET;
        my @file = <$fh>;
        foreach my $line (@file) {
            $line =~ s/$search/$replace/g;
        }
        close $fh;
        sysopen $fh, $file, O_WRONLY | O_TRUNC or
            die "Cannot write $file: $!\n";
        print $fh @file;
    }
    close $fh;
}
 
sub process_dir($) {
    my $dir = shift;
    my $dh = new IO::Handle;
    print "Entering $dir\n";
    opendir $dh, $dir or
        die "Cannot open $dir: $!\n";
    while(defined(my $cont = readdir($dh))) {
        next
            if $cont eq '.' || $cont eq '..';
        # Skip .swap files
        next
            if $cont =~ /^\.swap\./o;
        my $fullpath = File::Spec->catfile($dir, $cont);
        if($cont =~ /$search/) {
            my $newcont = $cont;
            $newcont =~ s/$search/$replace/g;
            print "  Renaming $cont to $newcont\n";
            rename $fullpath, File::Spec->catfile($dir, $newcont);
            $cont = $newcont;
            $fullpath = File::Spec->catfile($dir, $cont);
        }
        if(-l $fullpath) {
            my $link = readlink($fullpath);
            if($link =~ /$search/) {
                my $newlink = $link;
                $newlink =~ s/$search/$replace/g;
                print "  Relinking $cont from $link to $newlink\n";
                unlink $fullpath;
                my $res = symlink($newlink, $fullpath);
                warn "Symlink of $newlink to $fullpath failed\n"
                    unless $res;
            }
        }
        next
            unless -r $fullpath && -w $fullpath;
        if(-d $fullpath) {
            push @stack, $fullpath;
        } elsif(-f $fullpath) {
            process_file($fullpath);
        }
    }
    closedir($dh);
}
 
if(-f $startdir) {
    process_file($startdir);
} elsif(-d $startdir) {
    @stack = ($startdir);
    while(scalar(@stack)) {
        process_dir(shift(@stack));
    }
} else {
    die "$startdir is not a file or directory\n";
}
