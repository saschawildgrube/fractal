<?php
	
	require_once(GetSourceDir()."webservices_directory.inc");

	/*
	
	1. Delete test user(s)
	2. Add test user withOUT email validation
	3. Get test user (and check for validation status and user email)
	4. Attempt to authenticate the user (should be ok)
	5. Set email for the first time (with validation)
	6. Get test user (and check for validation status and user email)	
	7. Wait for a second
	8. Cleanup
	9. Validate email (by applying the token) should fail
	10. Get test user (and check for validation status and user email)	
	11. Set email a second time i.e. change it (with validation)
	12. Validate email
	13. Get test user (should be valiudated now)
	14. Set email a third time - i.e. change it (with validation)
	15. Get test user (and check for validation status and user email)	
	16. Wait for a second
	17. Cleanup
	18. Validate email (by applying the token) should fail
	19. Get test user (and check for validation status and user email)	
	
	CLEANUP
	* Delete Test User

	*/

	
	class CTest extends CUnitTest
	{
		private $m_strWebservice;
		private $m_strUserID;
		
		function __construct()
		{
			$this->m_strWebservice = "system/user";
			$arrayConfig = array();
			$arrayConfig["webservices"] = GetWebServicesDirectory();			
			parent::__construct("Web service system/user Email Validation Timeout (Post Signin)",$arrayConfig);
		}
		
		function OnInit()
		{
			$this->RequireWebservice($this->m_strWebservice);			 
			return parent::OnInit();
		}
		
		function OnTest()
		{
			parent::OnTest();
			
			$consumer = new CWebServiceConsumerWebApplication($this);

			$strUserName = "test-signin";
			$strEmail = "test@websitedevkit.com";
			$strEmail2 = "test2@websitedevkit.com";
			$strPassword = "secret";
		
			$this->Trace("1. DELETE USER:");
			$arrayParams = array();
			$arrayParams["user_name"] = $strUserName;
			$arrayParams["command"] = "delete";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			$this->Trace("");


			$this->Trace("2. ADD TEST USER WITHOUT EMAIL VALIDATION:");
			$arrayParams = array();
			$arrayParams["user_name"] = $strUserName;
			$arrayParams["password"] = $strPassword;
			$arrayParams["command"] = "add";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->m_strUserID = $consumer->GetResultValue("NEW_USER_ID");
			$this->Trace("New USERID = ".$this->m_strUserID);
			if ($this->m_strUserID == "")	
			{
				$this->Trace("Error: New user ID is empty!");		
				return;	
			}
			$this->m_strValidationToken = $consumer->GetResultValue("VALIDATIONTOKEN");
			$this->Trace("VALIDATIONTOKEN = \"".$this->m_strValidationToken."\"");
			if ($this->m_strValidationToken != "")	
			{
				$this->Trace("Error: Validation token should be empty!");		
				return;	
			}			
			$this->Trace("");
		
			$this->Trace("3. GET USER:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strUserNameCheck = $consumer->GetResultValue("USER","USER_NAME");
			if ($strUserNameCheck != $strUserName)	
			{
				$this->Trace("Error: \"$strUserNameCheck\" != \"$strUserName\"");		
				return;	
			}
			$strUserEmailCheck = $consumer->GetResultValue("USER","USER_EMAIL");
			if ($strUserEmailCheck != "")	
			{
				$this->Trace("Error: User email is \"$strUserEmailCheck\", but it must be empty.");		
				return;	
			}
			$strValidationTokenCheck = $consumer->GetResultValue("USER","USER_EMAIL_VALIDATION_TOKEN");
			if ($strValidationTokenCheck != "")	
			{
				$this->Trace("Error: Validation Token  \"$strValidationTokenCheck\" != \"\"");		
				return;	
			}
			$strValidationStatus = $consumer->GetResultValue("USER","USER_EMAIL_VALIDATION_STATUS");
			$this->Trace("USER_EMAIL_VALIDATION_STATUS = \"$strValidationStatus\"");
			if ($strValidationStatus != "NOTVALIDATED")	
			{
				$this->Trace("Error: Validation Status is not NOTVALIDATED");
				return;	
			}			
			$this->Trace("");		
		
		
			$this->Trace("4. AUTHENTICATE VIA PASSWORD:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["authenticationpayload"] = $strPassword;
			$arrayParams["command"] = "authenticate";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\" is unexpected.");		
				return;	
			}
			$this->Trace("");			



			$this->Trace("5. SET EMAIL FOR THE FIRST TIME (WITH VALIDATION):");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["user_email"] = $strEmail;
			$arrayParams["user_email_validation"] = 1; // activate email validation
			$arrayParams["user_email_validation_duration"] = 1; // 1 second to validate the email
			$arrayParams["command"] = "changeemail";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->m_strValidationToken = $consumer->GetResultValue("VALIDATION_TOKEN");
			$this->Trace("VALIDATION_TOKEN = \"".$this->m_strValidationToken."\"");
			if ($this->m_strValidationToken == "")	
			{
				$this->Trace("Error: Validation token should not be empty!");		
				return;	
			}			
			$this->Trace("");



			$this->Trace("6. GET USER:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strUserNameCheck = $consumer->GetResultValue("USER","USER_NAME");
			if ($strUserNameCheck != $strUserName)	
			{
				$this->Trace("Error: \"$strUserNameCheck\" != \"$strUserName\"");		
				return;	
			}
			$strUserEmailCheck = $consumer->GetResultValue("USER","USER_EMAIL");
			if ($strUserEmailCheck != "")	
			{
				$this->Trace("Error: User email is \"$strUserEmailCheck\", but it must be empty!");		
				return;	
			}
			$strValidationTokenCheck = $consumer->GetResultValue("USER","USER_EMAIL_VALIDATION_TOKEN");
			if ($strValidationTokenCheck != $this->m_strValidationToken)	
			{
				$this->Trace("Error: Validation token should be \"".$this->m_strValidationToken."\".");
				return;	
			}
			$strValidationStatus = $consumer->GetResultValue("USER","USER_EMAIL_VALIDATION_STATUS");
			$this->Trace("USER_EMAIL_VALIDATION_STATUS = \"$strValidationStatus\"");
			if ($strValidationStatus != "VALIDATION_INITIAL")	
			{
				$this->Trace("Error: Validation Status is not VALIDATION_INITIAL");
				return;	
			}			
			$this->Trace("");		



			$this->Trace("7. WAIT A SECOND");
			sleep(2);
			$this->Trace("");			



			$this->Trace("8. CLEANUP:");
			$arrayParams = array();
			$arrayParams["command"] = "cleanup";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");			


		
			$this->Trace("9. VALIDATE EMAIL:");
			$arrayParams = array();
			$arrayParams["user_email_validation_token"] = $this->m_strValidationToken;
			$arrayParams["command"] = "validateemail";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "TOKEN_NOT_FOUND")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\", but it should be TOKEN_NOT_FOUND");		
				return;	
			}
			$this->Trace("");			





			$this->Trace("10. GET USER:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strUserNameCheck = $consumer->GetResultValue("USER","USER_NAME");
			if ($strUserNameCheck != $strUserName)	
			{
				$this->Trace("Error: \"$strUserNameCheck\" != \"$strUserName\"");		
				return;	
			}
			$strUserEmailCheck = $consumer->GetResultValue("USER","USER_EMAIL");
			if ($strUserEmailCheck != "")	
			{
				$this->Trace("Error: User email is \"$strUserEmailCheck\", but it must be empty.");		
				return;	
			}
			$strValidationTokenCheck = $consumer->GetResultValue("USER","USER_EMAIL_VALIDATION_TOKEN");
			if ($strValidationTokenCheck != "")	
			{
				$this->Trace("Error: Validation token should be empty by now");		
				return;	
			}
			$strValidationStatus = $consumer->GetResultValue("USER","USER_EMAIL_VALIDATION_STATUS");
			$this->Trace("USER_EMAIL_VALIDATION_STATUS = \"$strValidationStatus\"");
			if ($strValidationStatus != "NOTVALIDATED")	
			{
				$this->Trace("Error: Validation Status is not NOTVALIDATED");
				return;	
			}			
			$this->Trace("");		




			$this->Trace("11. SET EMAIL (WITH VALIDATION):");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["user_email"] = $strEmail;
			$arrayParams["user_email_validation"] = 1; // activate email validation
			$arrayParams["user_email_validation_duration"] = 60; // sixty seconds to validate the email
			$arrayParams["command"] = "changeemail";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->m_strValidationToken = $consumer->GetResultValue("VALIDATION_TOKEN");
			$this->Trace("VALIDATION_TOKEN = \"".$this->m_strValidationToken."\"");
			if ($this->m_strValidationToken == "")	
			{
				$this->Trace("Error: Validation token should not be empty!");		
				return;	
			}			
			$this->Trace("");



			$this->Trace("12. VALIDATE EMAIL:");
			$arrayParams = array();
			$arrayParams["user_email_validation_token"] = $this->m_strValidationToken;
			$arrayParams["command"] = "validateemail";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\".");		
				return;	
			}
			$this->Trace("");			



			$this->Trace("13. GET USER:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strUserNameCheck = $consumer->GetResultValue("USER","USER_NAME");
			if ($strUserNameCheck != $strUserName)	
			{
				$this->Trace("Error: \"$strUserNameCheck\" != \"$strUserName\"");		
				return;	
			}
			$strUserEmailCheck = $consumer->GetResultValue("USER","USER_EMAIL");
			if ($strUserEmailCheck != $strEmail)	
			{
				$this->Trace("Error: User email is \"$strUserEmailCheck\", but it should be \"$strEmail\".");		
				return;	
			}
			$strValidationTokenCheck = $consumer->GetResultValue("USER","USER_EMAIL_VALIDATION_TOKEN");
			if ($strValidationTokenCheck != "")	
			{
				$this->Trace("Error: Validation token should be empty by now");		
				return;	
			}
			$strValidationStatus = $consumer->GetResultValue("USER","USER_EMAIL_VALIDATION_STATUS");
			$this->Trace("USER_EMAIL_VALIDATION_STATUS = \"$strValidationStatus\"");
			if ($strValidationStatus != "VALIDATED")	
			{
				$this->Trace("Error: Validation Status is not VALIDATED");
				return;	
			}			
			$this->Trace("");		



			$this->Trace("14. SET EMAIL FOR THE THIRD TIME - I.E. CHANGE IT (WITH VALIDATION):");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["user_email"] = $strEmail2;
			$arrayParams["user_email_validation"] = 1; // activate email validation
			$arrayParams["user_email_validation_duration"] = 1; // 1 second to validate the email
			$arrayParams["command"] = "changeemail";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->m_strValidationToken = $consumer->GetResultValue("VALIDATION_TOKEN");
			$this->Trace("VALIDATION_TOKEN = \"".$this->m_strValidationToken."\"");
			if ($this->m_strValidationToken == "")	
			{
				$this->Trace("Error: Validation token should not be empty!");		
				return;	
			}			
			$this->Trace("");



			$this->Trace("15. GET USER:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strUserNameCheck = $consumer->GetResultValue("USER","USER_NAME");
			if ($strUserNameCheck != $strUserName)	
			{
				$this->Trace("Error: \"$strUserNameCheck\" != \"$strUserName\"");		
				return;	
			}
			$strUserEmailCheck = $consumer->GetResultValue("USER","USER_EMAIL");
			if ($strUserEmailCheck != $strEmail)	
			{
				$this->Trace("Error: User email is \"$strUserEmailCheck\", but it must be \"$strEmail\"!");
				return;	
			}
			$strValidationTokenCheck = $consumer->GetResultValue("USER","USER_EMAIL_VALIDATION_TOKEN");
			if ($strValidationTokenCheck != $this->m_strValidationToken)	
			{
				$this->Trace("Error: Validation token should be \"".$this->m_strValidationToken."\".");
				return;	
			}
			$strValidationStatus = $consumer->GetResultValue("USER","USER_EMAIL_VALIDATION_STATUS");
			$this->Trace("USER_EMAIL_VALIDATION_STATUS = \"$strValidationStatus\"");
			if ($strValidationStatus != "VALIDATION_CHANGE")	
			{
				$this->Trace("Error: Validation Status is not VALIDATION_CHANGE");
				return;	
			}			
			$this->Trace("");		



			$this->Trace("16. WAIT A SECOND");
			sleep(2);
			$this->Trace("");			



			$this->Trace("17. CLEANUP:");
			$arrayParams = array();
			$arrayParams["command"] = "cleanup";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");			


		
			$this->Trace("18. VALIDATE EMAIL:");
			$arrayParams = array();
			$arrayParams["user_email_validation_token"] = $this->m_strValidationToken;
			$arrayParams["command"] = "validateemail";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "TOKEN_NOT_FOUND")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\", but it should be TOKEN_NOT_FOUND");
				return;	
			}
			$this->Trace("");			





			$this->Trace("19. GET USER:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strUserNameCheck = $consumer->GetResultValue("USER","USER_NAME");
			if ($strUserNameCheck != $strUserName)	
			{
				$this->Trace("Error: \"$strUserNameCheck\" != \"$strUserName\"");		
				return;	
			}
			$strUserEmailCheck = $consumer->GetResultValue("USER","USER_EMAIL");
			if ($strUserEmailCheck != $strEmail)	
			{
				$this->Trace("Error: User email is \"$strUserEmailCheck\", but it must be \"$strEmail\"");		
				return;	
			}
			$strValidationTokenCheck = $consumer->GetResultValue("USER","USER_EMAIL_VALIDATION_TOKEN");
			if ($strValidationTokenCheck != "")	
			{
				$this->Trace("Error: Validation token should be empty by now");		
				return;	
			}
			$strValidationStatus = $consumer->GetResultValue("USER","USER_EMAIL_VALIDATION_STATUS");
			$this->Trace("USER_EMAIL_VALIDATION_STATUS = \"$strValidationStatus\"");
			if ($strValidationStatus != "VALIDATED")	
			{
				$this->Trace("Error: Validation Status is not VALIDATED");
				return;	
			}			
			$this->Trace("");		
		
			$this->SetResult(true);
		}
		
		function OnCleanup()
		{
			parent::OnCleanup();
			
			$consumer = new CWebServiceConsumerWebApplication($this);
			
			if ($this->m_strUserID != "")
			{
				$this->Trace("DELETE USER:");
				$arrayParams = array();
				$arrayParams["user_id"] = $this->m_strUserID;
				$arrayParams["command"] = "delete";
				$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
				if ($consumer->GetError() != "")	
				{
					$this->Trace("Error: \"".$consumer->GetError()."\"");		
					return false;	
				}		
			}
	
			
			return true;
		}
		
		
	}
	
	

		
