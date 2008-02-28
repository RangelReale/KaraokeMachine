###############################################################################
# htmlize.perl
#   Pete Goodliffe
#   simple file munging to create the full set of TSE3 library documentation
###############################################################################

$version=@ARGV[0];

while ($line = <STDIN>)
{
    if (index($line,"<!--TSE3-VERSION-->") != -1)
    {
        print $version
    }
    elsif (index($line,"<!--TSE3-BODY-->") != -1)
    {
        print <<"EOB";
<body bgcolor=#ffffff text=#000000>
<table width=100% cellspacing=0 cellpadding=1 border=0 bgcolor=#000000><tr><td>
<table width=100% cellspacing=0 cellpadding=1 border=0><tr><td valign=center bgcolor=#c8d559>
<table width=100% cellspacing=0 cellpadding=0 border=0><tr>
<td align=left width=30%><b>&nbsp;TSE3 documentation<b></td>
<td align=center width=30%>Version $version</td>
<td align=right width=30%>
  <a href="index.html"><b>Index</b></a>
  <a href="api/index.html">API</a>&nbsp;
  <a href="Version.html">Version</a>&nbsp;
  <a href="Structure.html">Structure</a>&nbsp;
</td>
</tr></table>
</td></tr></table>
</td></tr></table>
EOB
    }
    elsif (index($line,"<!--TSE3-FOOTER-->") != -1)
    {
        print <<"EOB";
<body bgcolor=#ffffff text=#000000>
<table width=100% cellspacing=0 cellpadding=1 border=0 bgcolor=#000000><tr><td>
<table width=100% cellspacing=0 cellpadding=1 border=0><tr><td valign=center bgcolor=#c8d559>
<table width=100% cellspacing=0 cellpadding=0 border=0><tr>
<td align=left width=30%>&nbsp;&copy; Pete Goodliffe, 2001-2003</td>
<td align=center width=30%><a href="Copyright.html">Copyright</a></td>
<td align=right width=30%><a href="Psalm150.html">Psalm 150</a>&nbsp;</td>
</tr></table>
</td></tr></table>
</td></tr></table>
EOB
    }
    else
    {
        print $line;
    }
}
