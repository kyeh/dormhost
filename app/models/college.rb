class College < ActiveRecord::Base
  has_many :rooms
  has_many :profiles
  
  COLLEGES_LIST = College.find(:all, :order => "name").map{ |l| ["#{l.name}", l.id] }
  
  def is_updatable_by(user)
    user.is_admin?
  end
  
  def is_deletable_by(user)
    user.is_admin?
  end
  
  def self.is_readable_by(user, object = nil)
    true
  end
  
end
