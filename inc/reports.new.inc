<?php
    $bots_path = drupal_get_path('module', 'reports');
    $jquery_ui_path = drupal_get_path('module', 'jquery_ui');

    jquery_ui_add(array('ui.widget', 'ui.button', 'ui.mouse'));

    drupal_add_css($jquery_ui_path .'/jquery.ui/themes/redmond/jquery.ui.all.css');
    drupal_add_js($bots_path . '/js/jquery.uploadify.min.js');
    drupal_add_js($bots_path .'/js/new.js');
    //drupal_add_css($bots_path .'/css/new.css');
    drupal_add_css($bots_path . '/css/uploadify.css');

    function set_codesmith_csp_for_bot(){
        $json = json_decode($_POST['data']);
        if (!empty($json)){
            $bid = intval($json->bot);
            $dbid = intval($json->db);

            $sql = "SELECT jobid, bot_file_name, sector_project_name FROM mj_bots LEFT OUTER JOIN mj_sectors ON bts_sct_id = sct_id WHERE bts_id = $bid";

            $data = db_fetch_array(db_query($sql));

            $botname = $data['bot_file_name'];          //   must be a suitable name 
            $jobid = $data['jobid'];                    //   might be none, might be many
            $sector = $data['sector_project_name'];     //   got from database
            // getcwd(): D:\xampp\xampp\htdocs\drupal

            if(empty($botname)){
                echo 'Please set the file name for the bot';
                exit(0);
            }

            $tpl_path = getcwd() . '\\sites\\all\\modules\\reports\\new\\'; 

            $tpl_file = $tpl_path . 'tpl.csp';

            if (file_exists($tpl_file)){
                $output = file_get_contents($tpl_file); 
            }

            $sql = "SELECT dbname FROM  `mj_itg_dbs` WHERE dbs_id = $dbid";
            $data = db_fetch_array(db_query($sql));
            $database = $data['dbname'];  // optional

            if (!empty($json) && !empty($json->regex)){
                foreach ($json->regex as $key => $value){
                    $regex_str .= '<string>' . $value . '</string>';
                }
            }
            //$dir = $tpl_path . $sector . '\\' . $botname . '\\'; 
            $dir = $tpl_path . $botname . '\\'; 

            $output =   str_replace('$botname', $botname, $output); 
            $output =   str_replace('$jobid', $jobid, $output); 
            $output =   str_replace('$sector', $sector, $output); 
            $output =   str_replace('$database', $database, $output); 
            $output =   str_replace('$dir', $dir, $output); 
            $output =   str_replace('$regex', '<stringList>' . $regex_str . '</stringList>', $output); 

            if (!empty($json) && !empty($json->tables)){
                foreach ($json->tables as $key => $value){
                    if (empty($value->entity)){continue;}
                        $tables_str .= '<propertySet name="Entity" output="$(path)Entity\\'. $value->entity .'.cs" template="Entity.cst">
            <property name="NeedCollection">' . ($value->collection?'True':'False') . '</property>
            <property name="SourceTable">
                <connectionString>$(Conn)</connectionString>
                <providerType>$(pType)</providerType>
                <table><owner>dbo</owner><name>' . $value->table . '</name></table>
            </property>
            <property name="Sector">$(Sector)</property><property name="BotName">$(BotName)</property>
            <property name="EntityName">' . $value->entity . '</property>
            </propertySet>
            ';
                }
            }
            $output = str_replace('$entity', $tables_str, $output);
            $bot_csp = $tpl_path . '_' . $botname .'.csp';
            $target_zip = substr($dir, 0, -1) . '.zip';
            if (file_exists($target_zip)){
               // unlink($target_zip);      // remove the zip file if exists
               exec("rmdir $dir /s /q");    // remove the directory if exists
            }
            file_put_contents($bot_csp, $output);
            //var_dump($bot_csp);

            $retry = 60;
            $zip_created = false;
            while ($retry-->0){
                if(!file_exists($target_zip)){
                    sleep(1);
                    //echo 'sleeping...<br/>';
                } else {
                    //echo 'Find it~!' ; 
                    $zip_created = true; 
                    break; 
                }
            }
            if ($zip_created){
                echo ("./sites/all/modules/reports/new/$botname.zip");
            }
            exit(0);
        }

        // chdir('D:\\Program Files (x86)\\CodeSmith\\v6.5\\'); 
        // echo shell_exec("cs.exe "  . $bot_csp . " -v"); 
        // exec("cs.exe "  . $bot_csp . " -v", $data); 

        $tpl_dir = dirname(dirname(__FILE__)) . '\\tpl\\'; 
        $html = file_get_contents($tpl_dir . 'new_bot.tpl'); 
        //$tpl_dir = drupal_get_path('module', 'reports'); 
        $sql = 'select bts_id, botname from mj_bots order by id desc limit 0, 10 '; //limit 0, 20'; 
        $result = db_query($sql); 
        while ($data = db_fetch_object($result)){ 
            $bots[$data->bts_id]=$data->botname; 
        } 

        $sql = 'select dbs_id, dbname from mj_itg_dbs where valid = 1 order by dbname '; 
        $result = db_query($sql); 
        while ($data = db_fetch_object($result)){ 
            $dbs[$data->dbs_id]=$data->dbname; 
        } 

        $opl = '<option>'; 
        $opr = '</option>'; 
        foreach ($bots as $key => $value) { 
            $bots_html .= "<option value='$key'>" . $value . $opr; 
        } 
        foreach ($dbs as $key => $value) { 
            $dbs_html .= "<option value='$key'>" . $value . $opr; 
        } 

        $html = str_replace('$botname', $bots_html, $html); 
        $html = str_replace('$database', $dbs_html, $html); 
        return $html;
    }
?>