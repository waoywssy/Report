<?php
function deploy(){
	$result = '';
	$result .= t('Deploy by One Click:');
    $result .= t('<form id="deploy" name="deploy" action="#" method="post" />');
    $result .= t('&nbsp;&nbsp;<button id="btn_deploy" type="submit">Deploy</button>&nbsp;&nbsp;<br /><br />');
    $result .= t('</form>');
    return $result;
}