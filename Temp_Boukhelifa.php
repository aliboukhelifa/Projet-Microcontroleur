<?php 
header( "refresh:1" );
require_once ('jpgraph-4.3.4/src/jpgraph.php');
require_once ('jpgraph-4.3.4/src/jpgraph_line.php');


$db = new SQLite3('DataTemp.db');
$db->exec("CREATE TABLE IF NOT EXISTS Temperature(id INTEGER PRIMARY KEY, date_donnee DATE, donnee FLOAT)");

$fp = fopen("COM3", "w+");

if(!$fp) {
        echo "Error";
        die();
}else{
        sleep(1);
        $resultat = fread($fp,10);  
            
        while($resultat==NULL){
            $resultat = fread($fp,10);
        }
        $resultat = ord($resultat)/10;  //intval pr appli
        $date_temp = date("Y-m-d H:i:s");
        $db->exec("INSERT INTO Temperature(id, date_donnee, donnee) VALUES(NULL, '$date_temp', '$resultat')"); 
    
}

fclose($fp);

class MyDB extends SQLite3
{
    function __construct()
    {
        $this->open("DataTemp.db");
    }
}

$db = new MyDB();

$result = $db->query('SELECT * FROM Temperature');
$row = $result->fetchArray();

$datay1 = array();
$datay2 = array();


while ($row = $result->fetchArray()) {
    $datay1[] = $row['donnee'];
    $datay2[] = $row['date_donnee'];

}


//$datay1 = $arrayD;
//$datay2 = $arrayT;

// Setup the graph
$graph = new Graph(1000,1000);
$graph->SetScale("textlin", 0, 250);
$graph->img->SetMargin(100,40,60,100);

//$graph->img->SetMargin(100,40,60,100);

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
$graph->xaxis->SetTickLabels($datay2);
$graph->xaxis->SetTextLabelInterval(2);
$graph->xgrid->SetColor('#E3E3E3');

// Create the first line
$p1 = new LinePlot($datay1);
$p1->value->SetFormat('%d');
$p1->SetColor("#6495ED");
$graph->Add($p1);
$p1->SetLegend('Line 1');


$graph->legend->SetFrameWeight(1);

$graph->Stroke();

$url = $_SERVER['REQUEST_URI'];
//header("Refresh: 1; URL=$url");

// Output line
//$graph->Stroke("image.png");
//echo ('<img src="image.png">');

?>