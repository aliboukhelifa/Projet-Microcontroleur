<html>
<body>


<?php
//header( "refresh:1" );
//require_once ('jpgraph-4.3.4/src/jpgraph.php');
//require_once ('jpgraph-4.3.4/src/jpgraph_line.php');

exec('mode COM6: BAUD=9600 PARITY=N data=8 stop=1 xon=on');
$fp =fopen("COM6", "w+");

$db = new PDO('sqlite:Us.sqlite');

$db->exec("CREATE TABLE dataUs(
	id INTEGER PRIMARY KEY,
	distance INTEGER,
	heure TIME)"
);


$arrayD = array();
$arrayT = array();


if(!$fp) {
        echo "Error";die();
}

for($i=0; $i<=10; $i++){
    sleep(1);   
    $resulta = fread($fp,10);
    $today = date("H:i:s");

    $dist = ord($resulta);
    $db->exec("INSERT INTO dataUs(distance, heure) VALUES ('$dist','$today')");

    $arrayD[] = $dist;
    $arrayT[] = $today;

    echo ($today);
    echo ("  ");
    echo ord($resulta);
   
    echo "<br>";
    
}

fclose($fp);

?>

<?php  // content="text/plain; charset=utf-8"
require_once ('jpgraph-4.2.9/src/jpgraph.php');
require_once ('jpgraph-4.2.9/src/jpgraph_line.php');

$datay1 = $arrayD;
// Setup the graph
$graph = new Graph(900,900);
$graph->SetScale("textlin",0,250);
$graph->img->SetMargin(100,40,60,100);

$theme_class=new UniversalTheme;

$graph->SetTheme($theme_class);
$graph->img->SetAntiAliasing(false);
$graph->title->Set('Données Ultra Son');
$graph->SetBox(false);

$graph->SetMargin(40,20,36,63);

$graph->img->SetAntiAliasing();

$graph->yaxis->HideZeroLabel();
$graph->yaxis->HideLine(false);
$graph->yaxis->HideTicks(false,false);

$graph->xgrid->Show();
$graph->xgrid->SetLineStyle("solid");
$graph->xaxis->SetTickLabels($arrayT);
$graph->xaxis->SetTextLabelInterval(2);
$graph->xgrid->SetColor('#E3E3E3');

// Create the first line
$p1 = new LinePlot($arrayD);
$p1->value->SetFormat('%d');
$p1->SetColor("#6495ED");
$graph->Add($p1);
$p1->SetLegend('Line 1');


$graph->legend->SetFrameWeight(1);

// Output line
$graph->Stroke("image.png");
echo ('<img src="image.png">');

?>

</body>

</html>
