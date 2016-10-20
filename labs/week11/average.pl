#!/usr/bin/perl -w

while ($line = <STDIN>) {
	if ($line =~ /tlab 5000 (\w) (\w)/) {
		$order = $1;
		$insert_method = $2;
		$i = 0;
		while ($i < 7) {
			$newLine = <STDIN>;
			
			if ($i == 0 && $newLine =~ /#compares = (\d+)/){
				$results{$insert_method}{$order}{'Nb'} = $1;
			}
			
			if ($newLine =~ /depth = (\d+)/) {
				$results{$insert_method}{$order}{'depth'} = $1;
			}
			
			if ($newLine =~ /(\d+) values known to be/) {
				$results{$insert_method}{$order}{'Nk'} = $1;
			} 
			
			if ($newLine =~ /(\d+) values known to \*not/) {
				$results{$insert_method}{$order}{'Nu'} = $1;
			}
			
			if ($i == 6 && $newLine =~ /#compares = (\d+)/) {
				$results{$insert_method}{$order}{'Nbs'} = $1;
			}
			
			$i++;
		}
	}
}

foreach $insert (keys %results) {
	foreach $ord (keys $results{$insert}) {
		$averageCMP = ($results{$insert}{$ord}{'Nbs'} - $results{$insert}{$ord}{'Nb'}) / ($results{$insert}{$ord}{'Nk'} + $results{$insert}{$ord}{'Nu'});
		print "$insert $ord\ndepth: $results{$insert}{$ord}{'depth'}\ncomp: $averageCMP\n\n";
	}
}
