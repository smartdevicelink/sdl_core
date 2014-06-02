#! /usr/bin/perl

my $input = "";
my @defined;
my @undefined;
my @all;

while ($ARGV[0] =~ m/-(D|U)(.*)/)
{
  if ($1 eq "D")
  {
    push @defined, $2;
  }
  else
  {
    push @undefined, $2;
  }
  push @all, $2;
  shift;
}

while (<>)
{
  $input .= $_;
}

my $token_re = qr/
  (?<print>\/\*.*?\*\/) |
  (?<print>\s*\/\/[^\n]*) |
  (?<ifdef>(^|\n)[ \t]*\#[ \t]*ifdef\s+)(?<sym>[A-Za-z_][A-Za-z0-9_]*) |
  (?<ifndef>(^|\n)[ \t]*\#[ \t]*ifndef\s+)(?<sym>[A-Za-z_][A-Za-z0-9_]*) |
  (?<if>(^|\n)[ \t]*\#\s*if\s+)(?<condition>[^\n]*) |
  (?<endif>(^|\n)[ \t]*\#[ \t]*endif\b(\h*\/\/[^\n]*)?) |
  (?<else>(^|\n)[ \t]*\#[ \t]*else)\b |
  (?<print>\n) |
  (?<print>[^\n]*)
  /xs;

  my @levels; # 1 bit - ifdef/endif must be skipped
              # 2 bit - content must be skipped
              # 3 bit - skipping rule is inherited

  TOKEN:
  while($input =~ /$token_re/g)
  {
    if (defined($+{ifdef}))
    {
      if($+{sym} ~~ @all)
      {
        push (@levels, 1 | (($+{sym} ~~ @undefined) << 1) | ($levels[-1] & 2) | (($+{sym} ~~ @defined) << 3));
      }
      else
      {
        push (@levels, ($levels[-1] & 2) | 8);
        print "$+{ifdef}$+{sym}" unless $levels[-1] & 2;
      }
    }
    elsif (defined($+{ifndef}))
    {
      if($+{sym} ~~ @all)
      {
        push (@levels, 1 | (($+{sym} ~~ @defined) << 1) | ($levels[-1] & 2) | (($+{sym} ~~ @undefined) << 3));
      }
      else
      {
        push (@levels, ($levels[-1] & 2) | 8);
        print "$+{ifndef}$+{sym}" unless $levels[-1] & 2;
      }
    }
    elsif (defined($+{if}))
    {
      push @levels, ($levels[-1] & 2) | 8;
      print "$+{if}$+{condition}" unless $levels[-1] & 3;
    }
    if (defined($+{else}))
    {
      if (not (@levels[-1] & 8))
      {
        $skip = (pop @levels) ^ 2;
        push @levels, $skip;
      }
      print "$+{else}" unless $levels[-1] & 3;
    }
    elsif (defined($+{endif}))
    {
      $skip = pop @levels;
      print "$+{endif}" unless $skip & 3;
    }

    next TOKEN if $levels[-1] & 2;

    if (defined($+{print}))
    {
      print $+;
    }
}
