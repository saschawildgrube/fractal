<?php

	class CMyContent extends CContent
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$strText = u("Here are some German umlauts: ������");
			$strToken = u("�");
			$this->IncludeElement("highlight");
			$highlight = new CHighlightElement($this->GetWebsite(),"default");
			$arrayItems["ELEMENT_HIGHLIGHT"] = $highlight->RenderHighlight($strText,$strToken);

		}
	}
	