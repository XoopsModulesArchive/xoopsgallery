<?php
/*
 * $RCSfile: parser.inc,v $
 *
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2006 Bharat Mediratta
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA  02110-1301, USA.
 */
/**
 * @package Rewrite
 * @version $Revision: 1.2 $ $Date: 2006/01/10 04:42:17 $
 * @author Douglas Cau <douglas@cau.se>
 */

/* Required class */
GalleryCoreApi::requireOnce('modules/rewrite/classes/RewriteParser.class');

/* Status codes used by the Apache mod_rewrite parser */
define('REWRITE_STATUS_APACHE_NO_MOD_REWRITE', 11);
define('REWRITE_STATUS_APACHE_UNABLE_TO_TEST', 12);
define('REWRITE_STATUS_HTACCESS_MISSING', 13);
define('REWRITE_STATUS_HTACCESS_CANT_READ', 14);
define('REWRITE_STATUS_HTACCESS_CANT_WRITE', 15);
define('REWRITE_STATUS_EMBED_HTACCESS_MISSING', 16);
define('REWRITE_STATUS_EMBED_HTACCESS_CANT_READ', 17);
define('REWRITE_STATUS_EMBED_HTACCESS_CANT_WRITE', 18);

/**
 * This URL Rewrite parser provides Apache mod_rewrite support for short URLs.
 *
 *   http://httpd.apache.org/docs/2.2/mod/mod_rewrite.html
 *
 * @package Rewrite
 * @subpackage Parsers
 */
class ModRewriteParser extends RewriteParser {

    function ModRewriteParser() {
	$this->_setParserId('modrewrite');
	$this->_setParserType('preGallery');
	$this->_setUrlGeneratorId('ModRewriteUrlGenerator');
    }

    /**
     * @see RewriteParser::saveActiveRules
     */
    function saveActiveRules($activeRules=null, $rewriteModule=null) {
	GalleryCoreApi::requireOnce(
	    'modules/rewrite/classes/parsers/modrewrite/ModRewriteHelper.class');
	return ModRewriteHelper::saveActiveRules($this, $activeRules, $rewriteModule);
    }

    /**
     * @see RewriteParser::saveAccessList
     */
    function saveAccessList($accessList, $allowEmptyReferer) {
	GalleryCoreApi::requireOnce(
	    'modules/rewrite/classes/parsers/modrewrite/ModRewriteHelper.class');
	return ModRewriteHelper::saveAccessList($accessList, $allowEmptyReferer);
    }

    /**
     * @see RewriteParser::needsConfiguration
     */
    function needsConfiguration() {
	GalleryCoreApi::requireOnce(
	    'modules/rewrite/classes/parsers/modrewrite/ModRewriteHelper.class');
	return ModRewriteHelper::needsConfiguration();
    }

    /**
     * @see RewriteParser::getErrorMessage
     */
    function getErrorMessage($code, $rewriteModule=null) {
	if (!isset($rewriteModule)) {
	    list($ret, $rewriteModule) = GalleryCoreApi::loadPlugin('module', 'rewrite');
	    if ($ret) {
		return array($ret->wrap(__FILE__, __LINE__), null);
	    }
	}

	list ($ret, $message) = parent::getErrorMessage($code, $rewriteModule);
	if ($ret) {
	    return array($ret->wrap(__FILE__, __LINE__), null);
	}

	if (!isset($message)) {
	    switch ($code) {
	    case REWRITE_STATUS_HTACCESS_CANT_READ:
		$message = $rewriteModule->translate('Cannot read the .htaccess file');
		break;
	    case REWRITE_STATUS_HTACCESS_CANT_WRITE:
		$message = $rewriteModule->translate('Cannot write to the .htaccess file');
		break;
	    case REWRITE_STATUS_HTACCESS_MISSING:
		$message = $rewriteModule->translate('Cannot write to the .htaccess file, ' .
		    'please create it.');
		break;
	    case REWRITE_STATUS_EMBED_HTACCESS_CANT_READ:
		$message = $rewriteModule->translate('Cannot read the embedded .htaccess file');
		break;
	    case REWRITE_STATUS_EMBED_HTACCESS_CANT_WRITE:
		$message = $rewriteModule->translate('Cannot write to the embedded .htaccess file');
		break;
	    case REWRITE_STATUS_EMBED_HTACCESS_MISSING:
		$message = $rewriteModule->translate('Cannot write to the embedded .htaccess ' .
		    'file, please create it.');
		break;
	    default:
	        return array(GalleryCoreApi::error(ERROR_BAD_PARAMETER, __FILE__, __LINE__), null);
	    }
	}

	return array(null, $message);
    }

    /**
     * @see RewriteParser::loadTestResultsTemplate
     */
    function loadTestResultsTemplate(&$template, &$form) {
    	GalleryCoreApi::requireOnce(
	    'modules/rewrite/classes/parsers/modrewrite/ModRewriteHelper.class');
	return ModRewriteHelper::loadTestResultsTemplate($template, $form);
    }

    /**
     * @see RewriteParser::handleTestResultsRequest
     */
    function handleTestResultsRequest($form) {
	$error = $status = array();

	$ret = GalleryCoreApi::assertUserIsSiteAdministrator();
	if ($ret) {
	    return array($ret->wrap(__FILE__, __LINE__), null, null);
	}

	if (isset($form['force']['test1'])) {
	    $ret = GalleryCoreApi::setPluginParameter('module', 'rewrite',
		'modrewrite.status', serialize(array('needOptions' => false, 'forced' => 1)));
	    if ($ret) {
		return array($ret->wrap(__FILE__, __LINE__), null, null);
	    }
	}

	if (isset($form['force']['test2'])) {
	    $ret = GalleryCoreApi::setPluginParameter('module', 'rewrite',
		'modrewrite.status', serialize(array('needOptions' => true, 'forced' => 1)));
	    if ($ret) {
		return array($ret->wrap(__FILE__, __LINE__), null, null);
	    }
	}

	$status['saved'] = 1;
	return array(null, $error, $status);
    }

    /**
     * @see RewriteParser::loadAdminParserTemplate
     */
    function loadAdminParserTemplate(&$template, &$form) {
	GalleryCoreApi::requireOnce(
	    'modules/rewrite/classes/parsers/modrewrite/ModRewriteHelper.class');
	return ModRewriteHelper::loadAdminRewriteTemplate($template, $form);
    }

    /**
     * @see RewriteParser::handleAdminParserRequest
     */
    function handleAdminParserRequest($form) {
	$error = $status = array();

	GalleryCoreApi::requireOnce(
	    'modules/rewrite/classes/parsers/modrewrite/ModRewriteHelper.class');

	$ret = GalleryCoreApi::assertUserIsSiteAdministrator();
	if ($ret) {
	    return array($ret->wrap(__FILE__, __LINE__), null, null);
	}

	/* We can only save the embedded .htaccess in embedded mode */
	if (!GalleryUtilities::isEmbedded()) {
	    return array(GalleryCoreApi::error(ERROR_UNSUPPORTED_OPERATION, __FILE__, __LINE__),
		null, null);
	}

	if (empty($form['embeddedLocation']) || empty($form['embeddedHtaccess'])) {
	    $error[] = 'form[error][' . REWRITE_STATUS_EMPTY_VALUE . ']';
	    return array(null, $error, $status);
	}

	list ($ret, $code) = ModRewriteHelper::saveEmbedConfig(
	    array('embeddedLocation' => $form['embeddedLocation'],
		  'embeddedHtaccess' => $form['embeddedHtaccess']),
	    $this,
	    GalleryUtilities::getRequestVariables('controller') != 'SetupRewriteController');
	if ($ret) {
	    return array($ret->wrap(__FILE__, __LINE__), null, null);
	}

	if ($code == REWRITE_STATUS_OK) {
	    $status['saved'] = 1;
	} else {
	    $error[] = 'form[error][' . $code . ']';
	}

	return array(null, $error, $status);
    }

    /**
     * @see RewriteParser::needsEmbedConfig
     */
    function needsEmbedConfig() {
	foreach (array('embeddedLocation', 'embeddedHtaccess') as $key) {
	    list ($ret, $value) = GalleryCoreApi::getPluginParameter('module', 'rewrite',
		'modrewrite.' . $key);
	    if ($ret) {
		return array($ret->wrap(__FILE__, __LINE__), null);
	    }

	    if (empty($value)) {
		return array(null, true);
	    }
	}

	return array(null, false);
    }

    /**
     * @see RewriteParser::fetchEmbedConfig
     */
    function fetchEmbedConfig() {
	$config = array();
	foreach (array('embeddedLocation', 'embeddedHtaccess') as $key) {
	    list ($ret, $value) = GalleryCoreApi::getPluginParameter('module', 'rewrite',
		'modrewrite.' . $key);
	    if ($ret) {
		return array($ret->wrap(__FILE__, __LINE__), null);
	    }

	    $config[$key] = $value;
	}

	return array(null, $config);
    }

    /**
     * @see RewriteParser::fetchEmbedConfig
     */
    function saveEmbedConfig($param) {
    	GalleryCoreApi::requireOnce(
	    'modules/rewrite/classes/parsers/modrewrite/ModRewriteHelper.class');

	list ($ret, $code) = ModRewriteHelper::saveEmbedConfig($param, $this, true);
	if ($ret) {
	    return array($ret->wrap(__FILE__, __LINE__), null, null);
	}

	return array(null, $code,
	    ($code != REWRITE_STATUS_OK) ? $this->getErrorMessage($code) : null);
    }
}

?>
