{*
 * $Revision$
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="{$class}">
   {if count($theme.imageViews) > 1}
     {g->text text="Size: "}
     <select onchange="{literal}if (this.value) { newLocation = this.value; this.options[0].selected = true; location.href= newLocation; }{/literal}">
     {section name=imageView loop=$theme.imageViews}
       <option value="{g->url arg1="view=core.ShowItem" arg2="itemId=`$theme.item.id`"
	arg3="imageViewsIndex=`$smarty.section.imageView.index`"}"{if
	$smarty.section.imageView.index==$theme.imageViewsIndex} selected="selected"{/if}>
	 {if empty($theme.imageViews[imageView].width)}
	   {if isset($theme.imageViews[imageView].isSource)}
	     {g->text text="Source"}
	   {else}
	     {g->text text="Unknown"}
	   {/if}
	 {else}
	   {g->text text="%dx%d" arg1=$theme.imageViews[imageView].width
	       arg2=$theme.imageViews[imageView].height}
	 {/if}
       </option>
     {/section}
     </select>
     <br/>
   {/if}

   {if !empty($theme.sourceImage)}
     {g->text text="Full size (new window): "}
     {capture name="fullSize"}
       {if empty($theme.sourceImage.width)}
	 {$theme.sourceImage.itemTypeName.0}
       {else}
	 {g->text text="%dx%d" arg1=$theme.sourceImage.width
	     arg2=$theme.sourceImage.height}
       {/if}
     {/capture}
     {if count($theme.imageViews) > 1}
       <a href="{g->url arg1="view=core.DownloadItem" arg2="itemId=`$theme.item.id`"}" target=_blank>
	 {$smarty.capture.fullSize}
       </a>
     {else}
       {$smarty.capture.fullSize}
     {/if}
     <br/>
   {/if}
</div>
