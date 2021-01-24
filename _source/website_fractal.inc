<?php

	require_once('env.inc');
	require_once(GetWDKDir().'wdk_website.inc');
	
	class CFractalWebSite extends CWebSite
	{

		function __construct(
			$arrayParams)
		{
			
			$arrayMeta = array();
			$arrayMeta['version'] = '0.1'; 
			$arrayMeta['author'] = 'Sascha Wildgrube';
			$arrayMeta['copyright'] = '?TID_COPYRIGHT?';
			$arrayMeta['copyright_since'] = '2021';
			
			
			$arrayConfig = array();
			
			$arrayConfig['session_mode'] = SESSION_MODE_LOCAL_COOKIE;
			$arrayConfig['cache_deactivate'] = true;
			$arrayConfig['cache_allow_nocache'] = true;
			$arrayConfig['iframe_allow'] = false;

			$arrayConfig['check_unresolvedtid'] = true;
			$arrayConfig['check_invalidtid'] = true;
			$arrayConfig['check_unintendedoutput'] = true;
			
			$arrayConfig['languages'][] = 'EN'; 
			$arrayConfig['defaultcountry'] = 'DEU';
			$arrayConfig['countries'][] = 'DEU';
			
			
			$arrayConfig['layouts'][] = 'fractal';
			$arrayConfig['protocols'][] = 'https';
			$arrayConfig['admin_email'] = GetAdminEmail();
			
			$arrayConfig['seourl'] = 'seourl';
			$arrayConfig['opengraph'] = true;
			$arrayConfig['csvformat'] = 'excel';
			
			parent::__construct(
				GetRootURL(),
				GetSourceDir(),
				GetDocumentRootDir(),
				$arrayMeta,
				$arrayConfig,
				$arrayParams,
				'website_fractal');
					
		}
		
		function OnInitAssemblyLine(&$assemblyLine)
		{
			$assembly = new CAssembly($this,'fractal');
			$assemblyLine->AddAssembly($assembly);
	
			parent::OnInitAssemblyLine($assemblyLine);
		}

		function OnInitPlugins()
		{
			$arrayPlugins = array();
		
			$this->IncludePlugin('fractal');
			$arrayPlugins[] = new CFractalPlugin($this);;

			return $arrayPlugins;	
		}
		
		function OnInitRoutingMap()
		{
			parent::OnInitRoutingMap();
			$this->LoadRoutingMap('default');
		}
		
		function OnInit()
		{
			$bResult = parent::OnInit();
			
			// MONITORING
			$this->AddHeader('<!--AVAILABILITY-CHECK-->');
			
			// LAYOUT
			$this->SetLayout('fractal');
			$this->SetFramework('fullwidth');

			// CONTENT
			if ($this->GetContent() == '' || $this->GetContent() == 'index')
			{
				$this->SetContent('mandelbrot');	
			}

			// FORK ME ON GITHUB
			$this->IncludeElement('forkmeongithub');
			$forkmeongithub = new CForkMeOnGithubElement($this,'default');
			$this->SetOutputItem('FORKMEONGITHUB',$forkmeongithub->RenderForkMeOnGithub('saschawildgrube','fractal'));


			return $bResult;
		}
		
		function GetSiteName()
		{
			return 'Fractal';
		}
		
		function GetSiteTeamEmail()
		{
			return 'info@'.GetMailDomain();
		}

		function GetSiteTeamName()
		{
			return 'sascha@wildgrube.com';
		}

		function OnRenderNavigation($strOutput)
		{
			
			$navigationtree = new CNavigationTree();
			$navigationtree->LoadNavigationTree($this,'');
			$navigationtree->SetCurrentContext($this->GetContent());
			$navigationtree->SetContextParameterName('content');

			if (FindString($strOutput,'{NAVIGATION_HORIZONTALPATH}') != -1)
			{
				$navigation = new CNavigationElement($this,'horizontalpath');
				$this->SetOutputItem('NAVIGATION_HORIZONTALPATH',$navigation->RenderNavigationHorizontalPath($navigationtree));
	    }
         
			if (FindString($strOutput, '{NAVIGATION_FOOTER}') != -1)
			{
				$navigationtree_footer = new CNavigationTree();
				$navigationtree_footer->LoadNavigationTree(
					$this,
					'footer');
				$navigationtree_footer->SetCurrentContext($this->GetContent());
				$navigationtree_footer->SetContextParameterName('content');
         		
				$navigation = new CNavigationElement($this,'footer');
				$this->SetOutputItem('NAVIGATION_FOOTER',$navigation->RenderNavigationHorizontalPath($navigationtree_footer));
			}
		}


		function OnGetDateLocalFormat($strCountryID)
		{
				return '%Y-%m-%d';
		}

		function OnGetTimeLocalFormat($strCountryID)
		{
				return '%H:%M:%S';
		}

	}
	

