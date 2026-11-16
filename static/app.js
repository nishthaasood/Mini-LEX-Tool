const $ = (id) => document.getElementById(id);
const samples = JSON.parse($('samples').textContent);
const source = $('source');
function updateEditor() {
  const lines = source.value.split('\n').length;
  $('line-numbers').textContent = Array.from({length:lines},(_,i)=>i+1).join('\n');
  $('line-count').textContent = `${lines} LINE${lines === 1 ? '' : 'S'}`;
  $('char-count').textContent = `${source.value.length} CHARACTERS`;
}
source.addEventListener('input',updateEditor);
source.addEventListener('scroll',()=>{$('line-numbers').scrollTop=source.scrollTop;});
source.addEventListener('keydown',(event)=>{
  if(event.key==='Tab'){
    event.preventDefault();
    const start=source.selectionStart,end=source.selectionEnd;
    source.setRangeText('    ',start,end,'end');updateEditor();
  }
});
$('sample').addEventListener('click',()=>{source.value=samples[$('language').value];updateEditor();});
$('language').addEventListener('change',()=>{source.value=samples[$('language').value];updateEditor();});
$('clear').addEventListener('click',()=>{source.value='';updateEditor();source.focus();});
$('upload').addEventListener('click',()=>$('file').click());
$('file').addEventListener('change',async(event)=>{
  const file=event.target.files[0];if(!file)return;
  if(file.size>32000){alert('Please select a file smaller than 32 KB.');return;}
  source.value=await file.text();updateEditor();event.target.value='';
});
function node(tag,text,className){const n=document.createElement(tag);n.textContent=text;if(className)n.className=className;return n;}
function render(data){
  $('total').textContent=data.total;
  $('types').textContent=Object.keys(data.summary).length;
  $('errors-count').textContent=data.errors.length;
  $('result-label').textContent=data.errors.length?'COMPLETED WITH ERRORS':'ANALYSIS COMPLETE';
  const body=$('tokens');body.replaceChildren();
  if(!data.tokens.length){const tr=node('tr','');const td=node('td','No tokens found.','empty');td.colSpan=5;tr.append(td);body.append(tr);}
  data.tokens.forEach((t,i)=>{
    const tr=document.createElement('tr');
    [i+1,JSON.stringify(t.lexeme),t.type,t.line,t.column].forEach((value,j)=>{
      const td=document.createElement('td');
      if(j===2)td.append(node('span',value,'token-type'));else td.textContent=value;
      tr.append(td);
    });body.append(tr);
  });
  const summary=$('summary');summary.replaceChildren();summary.classList.remove('muted');
  const counts=Object.entries(data.summary);
  if(!counts.length)summary.append(node('span','No tokens found.'));
  counts.forEach(([type,count])=>{const row=node('div','','summary-row');row.append(node('span',type),node('strong',count));summary.append(row);});
  const diagnostics=$('diagnostics');diagnostics.replaceChildren();diagnostics.classList.remove('muted');
  if(!data.errors.length)diagnostics.append(node('span','✓ No lexical errors found.'));
  data.errors.forEach(e=>diagnostics.append(node('div',e,'diagnostic')));
}
$('analyze').addEventListener('click',async()=>{
  const btn=$('analyze');btn.disabled=true;btn.textContent='Analyzing...';
  try{
    const res=await fetch('/api/analyze',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({language:$('language').value,source:source.value})});
    const data=await res.json();if(!res.ok)throw Error(data.error||'Analysis failed');
    render(data);$('results').scrollIntoView({behavior:'smooth'});
  }catch(e){$('diagnostics').textContent=e.message;$('result-label').textContent='ANALYSIS FAILED';$('results').scrollIntoView({behavior:'smooth'});}
  finally{btn.disabled=false;btn.innerHTML='Analyze code <span aria-hidden="true">↗</span>';}
});
async function checkHealth(){
  try{const r=await fetch('/api/health');const data=await r.json();
    $('status-text').textContent=data.engine_ready?'Engine online':'Engine unavailable';
    $('status').classList.toggle('offline',!data.engine_ready);
  }catch(e){$('status-text').textContent='Server offline';$('status').classList.add('offline');}
}
source.value=samples.clike;updateEditor();checkHealth();
