<style>
table{border:none;}
/*
#loadingScreen {
	background: url(../images/loading.gif) no-repeat 5px 8px;
	padding-left: 25px;
}
*/
</style>
<span><b>Please make sure that you've already built the tables of the target bot on server 10.10.0.40, or the source code creation will be failed</b></span>
<div>
<table>
<tr><td><label>Latest Bots</label>&nbsp;</td><td><select id="bot">$botname</select></td></tr>
<tr><td><label>Database</label>&nbsp;</td><td><select id="db">$database</select></td></tr>
<tr><td><label>Regex Scripts</label>&nbsp;</td><td>
	<form>
		<div id="queue"></div>
		<input id="file_upload" name="file_upload" type="file" multiple="true">
		<input type="hidden" id="h_bid" value=0 />
	</form>
</td></tr>
<tr><td></td><td>
	<table id="regex-list"></table><span style="color:red;font-size:12px;">*Regex scripts should be named like 'Sector_Botname_RegexName.Regex'*</span>
</td>
</tr>
<tr><td><label>Table Script</label>&nbsp;</td><td>
	<form>
		<div id="queue1"></div>
		<input id="file_upload1" name="file_upload1" type="file" multiple="true">
		<input type="hidden" id="h_bid1" value=0 />
	</form>
</td></tr>
<tr><td><label>Table Entities</label>&nbsp;</td><td>
	<table id='table-entities'>
    <tr><th title='Table names'>Table</th><th title='The corresponding c# class names'>Entity</th><th>Collection</th></tr></table>
    </td></tr>
</table>
</div>
<button id="btnGetTpl">Get</button>
<div id="loadingScreen"></div>


