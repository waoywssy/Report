var targetPath = 'bots';
function check_id(){
	return [true, ''];
//	return [false,"The id has already been taken"];
}
function check_name(){
	return [true, ''];
//	return [false,"The name has already been taken"];
}

function check_version(version_value){
	$("#sector").attr('disabled', version_value == 'R20' ? '' : 'disabled');
}
function check_qa(checked){
	$("#qaFreq").attr('disabled', checked == 1 ? '' : 'disabled');
}
var on_version_change = function(e){
	check_version($(e.target).val());
}
var on_qa_change = function(e){
	check_qa($(e.target).val());
}
function enable_confirm_button(){
	$($('.ui-dialog-buttonset button').get(1)).attr('disabled', false).removeClass("ui-state-disabled");
	$('#confirm_msg').remove();
}

function scan(id, data){
	if (data=='false'){
		alert('error~');
		return;
	}
	var j = data['f'];
	var allchecked = true;
	var ignored_found = false;
	
	/*starts to list files*/
	var tmp = '<ul id="files">';
	for (var i = 0; i < j.length; i++){
		var is_checked = j[i]['checked'];
		tmp += '<li ';
		if (is_checked==null){
			tmp += ' class="firstfound" ';
		} else if (is_checked == '0'){
			tmp += ' class="ignored" ';
			ignored_found = true;  // now we have found ignored script
		}
		tmp += '><input type="checkbox" ' + (is_checked=='1'?'checked="checked"':'') + '"/>' + j[i]['file'] + '</li>';
		if (is_checked != '1'){
			allchecked = false; // for initialization
		}
	}
	tmp += '</ul>';
	tmp += '<span class="checkall"><input type="checkbox" id="checkall" ' + (allchecked ? 'checked="checked"':'') + '>All</span>';
	if (ignored_found){
		tmp += '<span><input type="checkbox" id="showignored">Show Hidden</span>';
	}
	tmp += '<br />';
	/*ends to list files*/
	
	$('#pop-up-tree').empty().append(tmp);	// generate new list each time
	
	// hide the ignored scripts 
	if (ignored_found){
		$('.ignored').hide();
		$('#showignored').bind('click', function(){
			this.checked ? $('.ignored').show() : $('.ignored').hide();
		});
	}
	
	/*****************************check all  begin**************************/
	$('#checkall').bind('click', function(){
			$('#files input').attr('checked', this.checked ? 'checked' : '');
			enable_confirm_button();
		});
		
	$('#files input').bind('click', function(){
		if (this.checked) {
			if ($('#files input').not(':checked').size() == 0) {
				$('#checkall').attr('checked', 'checked');
			}
		} else {
			$('#checkall').attr('checked', '');
		}
		enable_confirm_button();
	});

	// clicking the li should have the same effect as clicking the input
	$('#files li')
	.bind('click', function(event){
			var inpt = $('input', $(this));
			if (inpt[0]!==event.target){
				inpt.click();
				
				if (inpt[0].checked) {
					if ($('#files input').not(':checked').size() == 0) {
						$('#checkall').attr('checked', 'checked');
					}
				} else {
					$('#checkall').attr('checked', '');
				}
			}
	})
	.hover(
		function() { $(this).addClass('grid-hover'); },
		function() { $(this).removeClass('grid-hover');});;
}

jQuery().ready(function(){ 
	// Bots list
	var botsView = jQuery("#botsList").jqGrid({
		url:targetPath,
		datatype: "json",
		colNames:['Id', 'Bot Id','Bot Name', 'Sector', 'QA at Boryi?', 'QA Frequency','QA Time','Priority','JobId', 'Keywords', 'Version'], 
		colModel:[ 
				{name:'id', index:'id', width:0, editable: true, hidden:true, search:false, },
				{name:'bts_id', index:'bts_id', width:10, editable: true, editrules:{number:true, required:true, maxValue:99999, custom:true, custom_func:check_id,}, search:false, searchoptions:{sopt:['eq'], searchOnEnter:true}}, 
				{name:'botname', index:'botname', width:45, editable: true, editrules:{required:true, custom:true, custom_func:check_name,}, searchoptions:{sopt:['cn'], searchOnEnter:true}}, 
				{name:'sector', index:'sector', width:40, editable: true,edittype:"select",
						editoptions: { value: "1:E-Commerce;2:Retail - Softline ;3:Retail - Hardline ;4:Medical Devices;5:Telecommunications;6:Lodging;7:Auto Retail;8:Video Games;9:Real Estate;10:Store Locators;11:AirLines;12:R&D;13:Homebuilders;14:Mobile Phone;15:FExchange;16:Consumer Technology;20:Restaurant;19:KeywordSearches;22:Cable And Satellite"/*21:SocialNetworking;*/ },
						search:false, },
				{name:'qaAtBoryi', index:'qaAtBoryi', width:20, editable: true, edittype:"checkbox",editoptions: { value: "1:0", },search:false, defval:"1",}, 
				{name:'qaFreq', index:'version', width:30, editable: true, edittype:"select",
						editoptions: { value: "20:Daily;4:Weekly;8:Twice a Week;1:Monthly", 
										/*dataEvents: [{ type: 'change', fn: on_version_change },
													 { type: 'select', fn: on_version_change },	]*/
													 },
						search:false, 
				},
				{name:'qaTime', index:'qaTime', width:20, editable: true, align:"center",editrules:{number:true, required:false, maxValue:1.0, minValue:0,}, search:false, }, 
				{name:'priority', index:'priority', width:35, editable: true, edittype:"select",
						editoptions: { value: "1:Most Important;2:Very Important;3:Important;4:Less Important;5:Least Important"},search:false, }, 
				{name:'jobid', index:'jobid', width:30, editable: true, editrules:{/*required:true,*/}, searchoptions:{sopt:['cn'], searchOnEnter:true} },
				{name:'keywords', index:'keywords', width:40, editable: true, editrules:{required:true,}, search:false, },
				{name:'version', index:'version', width:30, editable: true, edittype:"select",
						editoptions: { value: "R20:R20;R16:R16;R14:R14;Ruby:Ruby", 
										dataEvents: [{ type: 'change', fn: on_version_change },
													 { type: 'select', fn: on_version_change },	]},
						search:false, 
				},
				],
		rowNum:20,
		autowidth:true,
		rowList:[15,30,60],
		pager:$('#botsPager'),
		gridComplete: function(){
			var ids = $("#botsList").jqGrid('getDataIDs');
			if (ids.length > 0) {
				$("#botid").attr("value", ids[0]);
				$('#botsList').jqGrid('setSelection',ids[0]);
			}
		},
		beforeInitData:function(){
/*			var cm = grid.jqGrid('getColProp', 'flag'),
			selRowId = grid.jqGrid('getGridParam', 'selrow');
			cm.editoptions.src = 'http://www.ok-soft-gmbh.com/img/flag_' + selRowId + '.gif';*/
		},
		loadComplete: function() {
			$("tr.jqgrow", this).contextMenu('myMenu1', {
				bindings: {
					'contextupload': function(trigger) {
						alert('You can upload maintenance for bot: ' + trigger.id + ' later');
						//	alert();
						// trigger is the DOM element ("tr.jqgrow") which are triggered
//						grid.editGridRow(trigger.id, {});
					},/*
					'add': function(//trigger
									) {
						grid.editGridRow("new", addSettings);
					},
					'del': function(trigger) {
						if ($('#del').hasClass('ui-state-disabled') === false) {
							// disabled item can do be choosed
							grid.delGridRow(trigger.id, delSettings);
						}
					}*/
				},
				onContextMenu: function(event) {
					var rowId = $(event.target).closest("tr.jqgrow").attr("id");
					//grid.setSelection(rowId);
					// disable menu for rows with even rowids
					$('#del').attr("disabled",Number(rowId)%2 === 0);
					if (Number(rowId)%2 === 0) {
						$('#del').attr("disabled","disabled").addClass('ui-state-disabled');
					} else {
						$('#del').removeAttr("disabled").removeClass('ui-state-disabled');
					}
					return true;
				}
			});
		},
		ondblClickRow:function(id){
			var rowData = jQuery('#botsList').jqGrid('getRowData', id);
			var bname = rowData.botname;
			var kword = rowData.keywords;

			if (kword == undefined || kword == ''){
				alert('One or more keywords required, seperated by ";"');
				return;
			}
			
			$.getJSON('scan', {bid:id, op:"scan"}, function(data){
				if (data == null){
					alert('Nothing found. Please refine the keywords or change the searching paths.');
					return;
				}
				
				scan(id, data);

				/*****************************check all  end**************************/
				$('#pop-up-tree').dialog({
					//autoOpen: true,
					modal: true,
					width:'auto',
					title:'Confirm files for ' + bname,
					buttons:{
						GetZippedFiles:function(){
							var files_checked = $('#files li').has('input:checked');
							if (files_checked.size() == 0){
								alert('No files to download! If no files related to the bot, try rescan');
								return;
							}
							$.ajax({
								  type:'POST',
								  url:'scan',
								  data:{bid: id, op:'download', checked:files_checked.text(),},
								  success: function(msg){
												//$('#pop-up-tree').append('<div id="download_link"><a href="' + msg + '">Download</a></div>');
//												window.location.href = $('#download_link a').attr('href');
												window.location.href = msg;
												//$(location).attr('href','http://www.boryi.com');
												//window.open('http://www.boryi.com');
												// use this to prevent from redirection
												//event.preventDefault();
											},
							});
						},
						Confirm:function(){
							$($('.ui-dialog-buttonset button').get(1)).attr('disabled', true).addClass("ui-state-disabled");
							var files_checked = $('#files li').has('input:checked');
							var	checked = files_checked.text();
							var	unchecked = $('#files li').not(files_checked).text();
							
							$.ajax({
							  type:'POST',
							  url:'scan',
							  data:{bid: id, op:'confirm', checked:checked, unchecked:unchecked},
							  success: function(msg){
											if(msg == 'true'){
												$('#confirm_msg').remove();
												$('#pop-up-tree').append('<span id="confirm_msg">Files confirmed!</span>');
												
												$.getJSON('scan', {bid:id, op:"scan"}, function(data){
													scan(id, data);
												});
											}
										},
							});
						},
						ReScan:function(){
							$.getJSON('scan', {bid:id, op:"scan"}, function(data){
								if (data == null){
									alert('Nothing found. Please refine the keywords or change the searching paths.');
									return;
								}
								scan(id, data);
								enable_confirm_button();
							});
					}}
				});
			})
		},
		height:'auto',
		viewrecords: true,
		sortname: 'botname',
		sortorder: "asc", 
		editurl:"managebots",
		caption:"View Bots",
		}
	);
	
	botsView.navGrid('#botsPager',{search:true,edit:true,add:true,del:false})

	// the add button events
	$("#add_botsList").unbind('click').click(function(){
		$("#botsList").jqGrid('editGridRow',"new",{
			height:'auto', 
			width:'200xp',
			reloadAfterSubmit:false,
			recreateForm:true, 
			closeAfterAdd:true,
		});
	});
	
	// the edit button events
	$("#edit_botsList").click(function(){
		var gr = $("#botsList").jqGrid('getGridParam','selrow');
		if( gr != null ) {
			$("#botsList").jqGrid('editGridRow',gr,{
				closeAfterEdit:true,
				beforeShowForm:function(){
						check_version($('#version').val());
						//check_version(0+($('#qaAtBoryi').attr('checked')==checked);
						// when change to previous record or next record
						$('#pData, #nData').bind('click', function(){
								check_version($('#version').val());
							});
					},
			});
		}
	});
	// the search button events
	$("#search_botsList").click(function(){
		// to fix the bug that called in drupal		
		$("#lui_botsList").remove();
		$(".data input.vdata").bind('keypress', function(event){
				if (event.which==13){
					//alert('enter');
					$(".ui-search").click();
				}
			}
		)
	});
})