#!/usr/bin/perl -w

for($i = 11 ; $i <= 100 ; $i++) {
    $command_ = sprintf("cp 001_BS.txt %03d_BS.txt",$i);
    printf("%s\n",$command_);
    system($command_);
}
