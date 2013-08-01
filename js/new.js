$(function() {
		// alert(document.cookie);
    $("button").button();

 		// file uploader toolkit
		$('#file_upload').uploadify({
			'formData'     	: {
				'timestamp' : '1358739522',
				'token'     : 'c376e347a876096d1fee91a9e64b77ac',
			},
			'auto'				: true,
			'buttonText'	: 'REGEX SCRIPTS',
			'swf'      		: 'uploadify.swf',
			'uploader' 		: 'uploadify',
			'fileSizeLimit'	: '1MB',
			'fileTypeExts'  : '*.regex;',
			'successTimeout': '10',
			'removeTimeout': '0',
			'removeCompleted' : true,
			onUploadError : function(file, errorCode, errorMsg){
				//alert(file + ' is not valid type - ' + errorMsg);
			},
			onUploadStart : function(file) {
        //$("#file_upload").uploadify("settings", "botid", $('#h_bid').val());
        $("#file_upload").uploadify("settings", "formData", {'new':1, 'type':'regex'});
        $("#regex-list").append("<tr><td>" + file.name + "</td></tr>");
      },
      onDialogClose: function(filesSelected, filesQueued, queueLength){
      	$("#regex-list").empty();
      }
    });

		$('#file_upload1').uploadify({
			'formData'     	: {
				'timestamp' : '1358739522',
				'token'     : 'c376e347a876096d1fee91a9e64b77ac',
			},
			'buttonText'	: 'SQL TABLES SCRIPT',
			'swf'      		: 'uploadify.swf',
			'uploader' 		: 'uploadify',
			'fileSizeLimit'	: '1MB',
			'fileTypeExts'  : '*.sql;',
			'successTimeout': '10',
			'removeTimeout': '1',
			'multi' : false,
			//'removeTimeout': '3',
			//'removeCompleted': false,
			onUploadError : function(file, errorCode, errorMsg){
				//alert(file + ' is not valid type - ' + errorMsg);
			},
			onUploadStart : function(file) {
        //$("#file_upload").uploadify("settings", "botid", $('#h_bid').val());
        $("#file_upload1").uploadify("settings", "formData", {'new':1, 'type':'sql'});
      },
      onUploadSuccess : function(file, data, response) {
				if (response){
					$("#table-entities tr:gt(0)").detach();
					$("#table-entities").append(data);	
				}
      }
    });

    $("button#btnGetTpl").click(function(){
				var regexlist = ''; 
				$('#regex-list td').each(function(){
				  regexlist += "\"" + $(this).html() + "\",";
					});

				if (regexlist.length > 0){
						regexlist = regexlist.substring(0, regexlist.length - 1);
				}

				var entites = [];
				var hasCollision = false;
				var tableentities = '';

      	$('#table-entities tr:gt(0)').each(function() {
      		var entity = $.trim($(this).find('input:text').val());
			    tableentities += "{\"table\":\"" + $.trim($(this).find('label').html()) + "\",";
			    tableentities += "\"entity\":\"" + entity + "\",";
			    tableentities += "\"collection\":" + ($(this).find(':checkbox').attr('checked')?'1':'0') + "},";

			    // check names collisions
			    if (entity!=''){
			    	var check = $.inArray(entity, entites); 
			    	if (check == "-1"){
			    		entites.push(entity);
			    	} else {
							hasCollision = true;
							// $(this).find('input:text').attr('style', 'color:red;');
			    		return;
			    	}
			    }
			 	});

      	// check any entity been set
			 	if(entites.length == 0){
      		alert('No entity names!');
      		return;
      	}

				// check if entity names has collision
      	if(hasCollision){
      		alert('Entity name must be unique!');
      		return;
      	}
		 		
		 		// check if table script loaded 
			 	if (tableentities=='') {
			 		alert('Please upload the tables script first!');
			 		return;
			 	} else {
	 				tableentities = tableentities.substring(0, tableentities.length - 1);
			 	}

				var params = '{"bot":' + $('#bot option:selected').val() + ',"db":' + $('#db option:selected').val() 
												+ ',"regex":[' + regexlist + '],"tables":[' + tableentities + ']}';
				$.ajax({
					type: 'POST',
					url: "new",
					data: 'data=' + params,
					complete: function(data) {
						if (data.responseText != ''){
              alert(data.responseText);
            }
          }
				});
    });
});
