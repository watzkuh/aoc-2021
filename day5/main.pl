use warnings;
use strict;

open(IN, $ARGV[0]) or die("Could not open: $!");

my @input = <IN>;
my %cloudMap;

foreach my $line (@input) {
  chomp($line);
  my ($from, $to) = split(/ -> /, $line);
  my ($x1, $y1) = split(/,/, $from);
  my ($x2, $y2) = split(/,/, $to);

  my ($start, $end);
  print "\n $x1 $y1 $x2 $y2";
  if ($x1 - $x2 == 0) { # y axis line
    if ($y1 < $y2) {
      ($start, $end) = ($y1, $y2);
    } else {
      ($start, $end) = ($y2, $y1);
    }
    for (my $i = $start; $i <= $end; $i++) {
      print "\n ($x1, 'x', $i)";
      $cloudMap{($x1, 'x', $i)}++;
    }
  } else {
    my $swappedX = 0;
    my $swappedY = 0;
    if ($x1 < $x2) {
      ($start, $end) = ($x1, $x2);
    } else {
      ($start, $end) = ($x2, $x1);
      $swappedX = 1;
    }
    if ($y2 > $y1) {
      ($y2, $y1) = ($y1, $y2);
      $swappedY = 1;
    }
    if ($y1 - $y2 == 0) { # x axis line
      for (my $i = $start; $i <= $end; $i++) {
        print "\n ($i, 'x', $y1)";
        $cloudMap{($i, 'x', $y1)}++;
      }
    }
    elsif ($start == $y1 and $end == $y2) { # diagonal increasing both
      for (my $i = $start; $i <= $end; $i++) {
        print "\n ($i, 'x', $i)";
        $cloudMap{($i, 'x', $i)}++;
      }
    }
    else { # reverse diagonal
      if ($swappedX and $swappedY) {
        for (my $i = $start, my $j = 0; $i <= $end; $i++, $j++) {
          print "\n ($i, 'x', ($y1 - $j))";
          $cloudMap{($i, 'x', ($y1 - $j))}++;
        }
      } elsif ($swappedX and !$swappedY) {
        for (my $i = $start, my $j = 0; $i <= $end; $i++, $j++) {
          print "\n ($i, 'x', ($y2 + $j))";
          $cloudMap{($i, 'x', ($y2 + $j))}++;
        }
      } elsif (!$swappedX and $swappedY) {
        for (my $i = $start, my $j = 0; $i <= $end; $i++, $j++) {
            print "\n ($i, 'x', ($y2 + $j))";
            $cloudMap{($i, 'x', ($y2 + $j))}++;
        }
      } elsif (!$swappedX and !$swappedY) {
        for (my $i = $start, my $j = 0; $i <= $end; $i++, $j++) {
            print "\n ($i, 'x', ($y1 - $j))";
            $cloudMap{($i, 'x', ($y1 - $j))}++;
        }
      }
    }
  }
}

my $overlaps = 0;
for (sort keys %cloudMap) {
  if ($cloudMap{$_} >= 2) {
    $overlaps++;
  }
}
print "\n$overlaps\n";