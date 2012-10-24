jQuery().ready(function(){
	$.getJSON('sectors', function(data){
		var j = data['f'];
		var tmp = '';
		for(var i = 0; i < j.length; i++){
			tmp += '<option value="' + j[i]['id'] + '">' + j[i]['sector']+ '</option>'
		}
		$('#sectors').append(tmp);

		if ($('#sectors option').size() > 0){
			
			$('#sectors').bind('change', function(){
				$('#newpath').val('');
				var id = $('#sectors').find('option:selected').val();
				$.getJSON('path', {sid: id}, function(data){
					var len = data.length;
					var tmp = '<ul id="pathul">';
					
					for (var i = 0; i < len; i++){
						tmp += '<li><span class="del"><button type="button">Delete</button></span><span class="path">' + data[i] + '</span></li>';
					}
					tmp += '</ul>';
					$('#paths').empty().append(tmp);
					var lis = $('#paths li');
					if (lis.size() > 0){
						lis.hover(
							function(){$(this).addClass('grid-hover')/*.find('.del').show()*/;}, 
							function(){$(this).removeClass('grid-hover')/*.find('.del').hide()*/;});
							
						lis.find('button').bind('click', function(){
							var path = $(this).parent().siblings('span').text();
							if(confirm('You\'ll delete: ' + $(this).parent().siblings('span').text())){
								// json request here
								$.ajax({
									  type:'POST',
									  url:'setpath',
									  data:{sid:id, p:path.replace(';', ''), op:'del'},
									  success:function(){
										  //$('#sectors').trigger('change');
									  }
								});
								$(this).parent().parent().remove();
							}
						});
					}
				});
			}).trigger('change');
			
			// a simple regex to validate Windows directory
			var reg = /^[c-zC-Z]:(\\[a-zA-Z0-9\._-]+)+\\?$/i;			
			// add button
			$('#add').bind('click', function(){
					var path = $('#newpath').val();
					if(reg.test(path)){
						var id = $('option:selected').val();
						$.ajax({
							  type:'POST',
							  url:'setpath',
							  data:{sid:id, p:path, op:'add'},
							  success:function(){
								  // display the new one
								  $('#sectors').trigger('change');
							  }
						});
					}else{
						alert('Not a valid path');
					}
				});
			$('#newpath').keypress(function(event){
					if (event.which == 13) {
						event.preventDefault();
						$('#add').trigger('click');
					}
				});
		}
	});
});