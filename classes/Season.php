<?php
class Season{
	private $id;
	private $showId;
	private $number;
	private $name;
	private $tvdbId;
	private $tvrageId;

	public function __construct($id, $showId, $number, $name, $tvdbId, $tvrageId){
		$this->id = $id;
		$this->showId = $showId;
		$this->number = $number;
		$this->name = $name;
		$this->tvdbId = $tvdbId;
		$this->tvrageId = $tvrageId;
	}

  /**
   * Gets the value of id.
   *
   * @return mixed
   */
  public function getid() {
    return $this->id;
  }

  /**
   * Sets the value of id.
   *
   * @param mixed $id the id
   * @return self
   */
  public function setid($id) {
    $this->id = $id;
    return $this;
  }
  /**
   * Gets the value of showId.
   *
   * @return mixed
   */
  public function getshowId() {
    return $this->showId;
  }

  /**
   * Sets the value of showId.
   *
   * @param mixed $showId the showId
   * @return self
   */
  public function setshowId($showId) {
    $this->showId = $showId;
    return $this;
  }
  /**
   * Gets the value of number.
   *
   * @return mixed
   */
  public function getnumber() {
    return $this->number;
  }

  /**
   * Sets the value of number.
   *
   * @param mixed $number the number
   * @return self
   */
  public function setnumber($number) {
    $this->number = $number;
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
   * @return mixed
   */
  public function gettvdbId() {
    return $this->tvdbId;
  }

  /**
   * Sets the value of tvdbId.
   *
   * @param mixed $tvdbId the tvdbId
   * @return self
   */
  public function settvdbId($tvdbId) {
    $this->tvdbId = $tvdbId;
    return $this;
  }
  /**
   * Gets the value of tvrageId.
   *
   * @return mixed
   */
  public function gettvrageId() {
    return $this->tvrageId;
  }

  /**
   * Sets the value of tvrageId.
   *
   * @param mixed $tvrageId the tvrageId
   * @return self
   */
  public function settvrageId($tvrageId) {
    $this->tvrageId = $tvrageId;
    return $this;
  }}
?>