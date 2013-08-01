<?php
class Show {
	private $id;
	private $name;
	private $tvdbId;
	private $tvrageId;
	private $cached;

	private $startDate;
	private $endDate;
	private $status;
	private $originCountry;
	private $runtime;

	public function __construct($id, $name, $tvdbId, $tvrageId, $cached) {
		$this->id = $id;
		$this->name = $name;
		$this->tvdbId = $tvdbId;
		$this->tvrageId = $tvrageId;
	}

	public function setMeta($startDate, $endDate, $status, $originCountry, $runtime) {

	}

  /**
   * Gets the value of id.
   *
   * @return int
   */
  public function getid() {
    return $this->id;
  }

  /**
   * Sets the value of id.
   *
   * @param int $id the id
   * @return self
   */
  public function setid($id) {
    $this->id = $id;
    return $this;
  }
  /**
   * Gets the value of name.
   *
   * @return string
   */
  public function getname() {
    return $this->name;
  }

  /**
   * Sets the value of name.
   *
   * @param string $name the name
   * @return self
   */
  public function setname($name) {
    $this->name = $name;
    return $this;
  }
  /**
   * Gets the value of tvdbId.
   *
   * @return int
   */
  public function gettvdbId() {
    return $this->tvdbId;
  }

  /**
   * Sets the value of tvdbId.
   *
   * @param int $tvdbId the tvdbId
   * @return self
   */
  public function settvdbId($tvdbId) {
    $this->tvdbId = $tvdbId;
    return $this;
  }
  /**
   * Gets the value of tvrageId.
   *
   * @return int
   */
  public function gettvrageId() {
    return $this->tvrageId;
  }

  /**
   * Sets the value of tvrageId.
   *
   * @param int $tvrageId the tvrageId
   * @return self
   */
  public function settvrageId($tvrageId) {
    $this->tvrageId = $tvrageId;
    return $this;
  }
  /**
   * Gets the value of cached.
   *
   * @return int
   */
  public function getcached() {
    return $this->cached;
  }

  /**
   * Sets the value of cached.
   *
   * @param int $cached the cached
   * @return self
   */
  public function setcached($cached) {
    $this->cached = $cached;
    return $this;
  }
  /**
   * Gets the value of startDate.
   *
   * @return string
   */
  public function getstartDate() {
    return $this->startDate;
  }

  /**
   * Sets the value of startDate.
   *
   * @param string $startDate the startDate
   * @return self
   */
  public function setstartDate($startDate) {
    $this->startDate = $startDate;
    return $this;
  }
  /**
   * Gets the value of endDate.
   *
   * @return string
   */
  public function getendDate() {
    return $this->endDate;
  }

  /**
   * Sets the value of endDate.
   *
   * @param string $endDate the endDate
   * @return self
   */
  public function setendDate($endDate) {
    $this->endDate = $endDate;
    return $this;
  }
  /**
   * Gets the value of status.
   *
   * @return string
   */
  public function getstatus() {
    return $this->status;
  }

  /**
   * Sets the value of status.
   *
   * @param string $status the status
   * @return self
   */
  public function setstatus($status) {
    $this->status = $status;
    return $this;
  }
  /**
   * Gets the value of originCountry.
   *
   * @return string
   */
  public function getoriginCountry() {
    return $this->originCountry;
  }

  /**
   * Sets the value of originCountry.
   *
   * @param string $originCountry the originCountry
   * @return self
   */
  public function setoriginCountry($originCountry) {
    $this->originCountry = $originCountry;
    return $this;
  }
  /**
   * Gets the value of runtime.
   *
   * @return int
   */
  public function getruntime() {
    return $this->runtime;
  }

  /**
   * Sets the value of runtime.
   *
   * @param int $runtime the runtime
   * @return self
   */
  public function setruntime($runtime) {
    $this->runtime = $runtime;
    return $this;
  }}
?>